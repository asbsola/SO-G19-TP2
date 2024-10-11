#include "def.h"
#include <idle.h>
#include <utils/string.h>
#include <registers.h>
#include <managers/processManager.h>

#define MAX(a, b) ((a)<(b)?(b):(a))

extern void start_process_wrapper();
extern void go_to_scheduler();

char** copy_argv(processManagerADT processes_manager, char** argv);
void free_argv(processManagerADT process_manager, char** argv);
char* fallback[] = { NULL };

struct processManagerCDT
{
    uint64_t num_processes;
    uint64_t max_pid;
    memoryManagerADT memory_manager;
    schedulerADT scheduler;
    processControlBlockADT processes[MAX_PROCESSES];
};

typedef struct startFrame
{
    processManagerADT process_manager;
    pid_t pid;
    uint64_t (*process_start)(char **, int);
    char **argv;
} startFrame;

int get_argc(char* argv[]){
    int argc;
    for (argc = 0; argv[argc] != NULL; argc++);
    return argc;
}

void start_process(startFrame* data) {
    int argc = get_argc(data->argv);

    int64_t status = data->process_start(data->argv, argc);
    exit_process(data->process_manager, data->pid, status);
}

processManagerADT init_process_manager(memoryManagerADT memory_manager, schedulerADT scheduler)
{
    processManagerADT process_manager = mem_alloc(memory_manager, sizeof(struct processManagerCDT));
    if (process_manager == NULL)
        return NULL;

    for (pid_t pid = 0; pid < MAX_PROCESSES; pid++)
        process_manager->processes[pid] = NULL;

    process_manager->memory_manager = memory_manager;
    process_manager->scheduler = scheduler;
    process_manager->num_processes = 0;
    process_manager->max_pid = 0;

    char* argv[] = {"idle", NULL};
    create_process(process_manager, -1, NOT_IN_FOREGROUND, idle, argv);

    return process_manager;
}

pid_t get_lowest_unused_pid(processManagerADT process_manager){
    pid_t pid;
    for (pid = IDLE_PROCESS_PID; process_manager->processes[pid] != NULL && pid <= process_manager->max_pid; pid++);
    return pid;
}

pid_t create_process(processManagerADT process_manager, pid_t parent_pid, uint8_t is_in_foreground, uint64_t (*process_start)(char**, int), char** argv) {
    if (process_manager->num_processes >= MAX_PROCESSES)
        return -1;

    processControlBlockADT process_pcb = mem_alloc(process_manager->memory_manager, sizeof(processControlBlockCDT));
    char *stack = mem_alloc(process_manager->memory_manager, PROCESS_STACK_SIZE);

    if (process_pcb == NULL || stack == NULL)
        return -1;

    pid_t pid = get_lowest_unused_pid(process_manager);

    char** argv_copy = copy_argv(process_manager, argv);

    process_pcb->pid = pid;
    process_pcb->stack = stack;
    process_pcb->parent_pid = parent_pid;
    process_pcb->status = READY;
    process_pcb->priority = LOW;
    process_pcb->is_waiting = NOT_WAITING;
    process_pcb->is_in_foreground = is_in_foreground;
    process_pcb->argv = argv_copy;

    struct startFrame *start_frame = (startFrame *)(process_pcb->stack + PROCESS_STACK_SIZE - sizeof(startFrame));
    start_frame->process_manager = process_manager;
    start_frame->process_start = process_start;
    start_frame->pid = pid;
    start_frame->argv = argv_copy;

    registers64_t* call_frame = (registers64_t*) (process_pcb->stack + PROCESS_STACK_SIZE - sizeof(startFrame) - sizeof(registers64_t)); 
    call_frame->rip = (uint64_t)start_process_wrapper;
    call_frame->rsp = (uint64_t)(process_pcb->stack + PROCESS_STACK_SIZE - sizeof(startFrame));
    call_frame->ss = 0x0;
    call_frame->cs = 0x8;
    call_frame->rflags = 0x202;

    process_pcb->rsp = (uint64_t)(process_pcb->stack + PROCESS_STACK_SIZE - sizeof(startFrame) - sizeof(registers64_t));

    process_manager->processes[pid] = process_pcb;
    process_manager->num_processes++;
    process_manager->max_pid = MAX(process_manager->max_pid, pid);

    schedule_process(process_manager->scheduler, process_pcb);

    return process_pcb->pid;
}

void check_parent_waiting(processManagerADT process_manager, pid_t pid) {
    pid_t parent_pid = process_manager->processes[pid]->parent_pid;

    if (process_manager->processes[parent_pid]->is_waiting == WAITING)
        unblock_process(process_manager, parent_pid);
}

int exit_process(processManagerADT process_manager, pid_t pid, int64_t status)
{
    if (pid == IDLE_PROCESS_PID || pid >= MAX_PROCESSES || process_manager->processes[pid] == NULL)
        return -1;

    process_manager->processes[pid]->status = EXITED;
    process_manager->processes[pid]->ret = status;

    check_parent_waiting(process_manager, pid);


    for (pid_t i = 0; i <= process_manager->max_pid; i++)
        if (process_manager->processes[i] != NULL && process_manager->processes[i]->parent_pid == pid)
            process_manager->processes[i]->parent_pid = IDLE_PROCESS_PID;

    if(get_current_process(process_manager->scheduler) == pid)
        go_to_scheduler();
    else
        deschedule_process(process_manager->scheduler, process_manager->processes[pid]);
    return 0;
}

int block_process(processManagerADT process_manager, pid_t pid)
{
    if (pid >= MAX_PROCESSES || process_manager->processes[pid] == NULL)
        return -1;

    process_manager->processes[pid]->status = BLOCKED;

    if(get_current_process(process_manager->scheduler) == pid)
        go_to_scheduler();
    else
        deschedule_process(process_manager->scheduler, process_manager->processes[pid]);
    return 0;
}

int unblock_process(processManagerADT process_manager, pid_t pid)
{
    if (pid >= MAX_PROCESSES || process_manager->processes[pid] == NULL)
        return -1;

    process_manager->processes[pid]->status = READY;
    schedule_process(process_manager->scheduler, process_manager->processes[pid]);
    return 0;
}

int remove_process(processManagerADT process_manager, pid_t pid){
    if(process_manager->processes[pid] == NULL)
        return -1;

    free_argv(process_manager, process_manager->processes[pid]->argv);
    mem_free(process_manager->memory_manager, process_manager->processes[pid]->stack);
    mem_free(process_manager->memory_manager, process_manager->processes[pid]);

    process_manager->processes[pid] = NULL;
    process_manager->num_processes--;
    if(pid == process_manager->max_pid){
        int i;
        for(i=pid-1; i>=0 && process_manager->processes[i] == NULL; i--);
        process_manager->max_pid = i;
    }
    return 0;
}

int kill_process(processManagerADT process_manager, pid_t pid) {
    if (pid == IDLE_PROCESS_PID || pid >= MAX_PROCESSES || process_manager->processes[pid] == NULL)
        return -1;

    for (pid_t pid_i = 0; pid_i <= process_manager->max_pid; pid_i++)
        if (process_manager->processes[pid_i] != NULL && process_manager->processes[pid_i]->parent_pid == pid)
            kill_process(process_manager, pid_i);

    char was_ready = process_manager->processes[pid]->status == READY;
    process_manager->processes[pid]->status = KILLED;

    check_parent_waiting(process_manager, pid);

    if(get_current_process(process_manager->scheduler) == pid){
        remove_process(process_manager, pid);
        go_to_scheduler();
    }
    else{
        if(was_ready) deschedule_process(process_manager->scheduler, process_manager->processes[pid]);
        remove_process(process_manager, pid);
    }
    return 0;
}

int has_children(processManagerADT process_manager, pid_t my_pid) {
    for (pid_t pid = 0; pid <= process_manager->max_pid; pid++) {
        if (process_manager->processes[pid] == NULL) continue;

        if (process_manager->processes[pid]->parent_pid == my_pid)
            return 1;
    }

    return 0;
}

int wait(processManagerADT process_manager){
    pid_t my_pid = get_current_process(process_manager->scheduler);

    if (!has_children(process_manager, my_pid))
        return -1;

    process_manager->processes[my_pid]->is_waiting = WAITING;
    block_process(process_manager, my_pid);

    for (pid_t pid = 0; pid <= process_manager->max_pid; pid++) {
        processControlBlockADT pcb = process_manager->processes[pid];
        if (pcb == NULL) continue;

        if (pcb->parent_pid == my_pid) {
            uint64_t ret = pcb->ret;
            switch (pcb->status) {
                case EXITED:
                    remove_process(process_manager, pcb->pid);
                    return ret;
                case KILLED:
                    return -1;
                default:
                    break;
            }
        }
    }

    return -1;
}

processControlBlockADT* get_processes(processManagerADT process_manager) {
    return process_manager->processes;
}

uint64_t get_num_processes(processManagerADT process_manager){
    return process_manager->num_processes;
}

uint64_t get_max_pid(processManagerADT process_manager){
    return process_manager->max_pid;
}

uint64_t get_ps_data(processManagerADT process_manager, memoryManagerADT mem_manager) {
    process_info_t * processes = (process_info_t *) mem_alloc(mem_manager, sizeof(process_info_t) * (process_manager->num_processes + 1));
    int index = 0;
    for(int i = 0; i <= process_manager->max_pid; i++) {
        if(process_manager->processes[i] != NULL){
            processes[index].pid = process_manager->processes[i]->pid;
            processes[index].parent_pid = process_manager->processes[i]->parent_pid;
            processes[index].priority = process_manager->processes[i]->priority;
            processes[index].stack_pointer = (void *)process_manager->processes[i]->rsp;
            processes[index].base_pointer = process_manager->processes[i]->stack;
            processes[index].status = process_manager->processes[i]->status;
            processes[index].is_in_foreground = process_manager->processes[i]->is_in_foreground;
            processes[index].name = process_manager->processes[i]->argv[0];
            index++;
        }
    }
    processes[index].pid = -1;

    return (uint64_t)processes;
}

char** copy_argv(processManagerADT processes_manager, char** argv) {
    uint64_t size_of_argv;
    for (size_of_argv = 0; argv[size_of_argv] != NULL; size_of_argv++);

    char** argv_copy = (char**)mem_alloc(processes_manager->memory_manager, (size_of_argv + 1) * sizeof(char*));

    if (argv_copy == NULL)
        return fallback;

    for (uint64_t i = 0; i < size_of_argv; i++) {
        uint64_t len = str_len(argv[i]);

        char* arg = (char*)mem_alloc(processes_manager->memory_manager, (len + 1) * sizeof(char));
        if (arg == NULL) {
            for (uint64_t j = 0; j < i; j++)
                mem_free(processes_manager->memory_manager, argv_copy[j]);

            mem_free(processes_manager->memory_manager, argv_copy);
            return fallback;
        }

        str_cpy(arg, argv[i]);
        argv_copy[i] = arg;
    }

    argv_copy[size_of_argv] = NULL;
    return argv_copy;
}

void free_argv(processManagerADT process_manager, char** argv) {
    for (uint64_t i = 0; argv[i] != NULL; i++)
        mem_free(process_manager->memory_manager, argv[i]);

    mem_free(process_manager->memory_manager, argv);
}


uint64_t nicent(processManagerADT process_manager, pid_t pid, processPriority priority){
    if( pid == IDLE_PROCESS_PID || pid >= MAX_PROCESSES || process_manager->processes[pid] == NULL || process_manager->processes[pid]->status == EXITED || process_manager->processes[pid]->status == KILLED)
        return -1;
    
    if(process_manager->processes[pid]->status == BLOCKED){
        process_manager->processes[pid]->priority = priority;
        return 0;
    }
    change_process_priority(process_manager->scheduler, process_manager->processes[pid], process_manager->processes[pid]->priority, priority);
    return 0;
}
