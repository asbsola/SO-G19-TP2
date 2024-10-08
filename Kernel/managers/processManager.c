#include <managers/processManager.h>
#include <registers.h>
#include <stdint.h>

extern void (*start_wrapper)();
extern void (*go_to_scheduler)();

struct processManagerCDT
{
    uint64_t num_processes;
    memoryManagerADT memory_manager;
    processControlBlockADT processes[MAX_PROCESSES];
};

typedef struct startFrame
{
    processManagerADT process_manager;
    pid_t pid;
    uint64_t (*process_start)(char **, int);
    char **argv;
} startFrame;

void start(startFrame *data)
{
    int argc;
    for (argc = 0; data->argv[argc] != NULL; argc++)
        ;

    exit_process(data->process_manager, data->pid, data->process_start(data->argv, argc));
}

processManagerADT init_process_manager(memoryManagerADT memory_manager)
{
    processManagerADT process_manager = mem_alloc(memory_manager, sizeof(struct processManagerCDT));

    if (process_manager == NULL)
        return NULL;

    for (pid_t pid = 0; pid < MAX_PROCESSES; pid++)
        process_manager->processes[pid] = NULL;

    process_manager->memory_manager = memory_manager;
    process_manager->num_processes = 0;
    return process_manager;
}

pid_t create_process(processManagerADT process_manager, pid_t parent_pid, uint64_t (*process_start)(char **, int), char **argv)
{
    if (process_manager->num_processes >= MAX_PROCESSES)
        return -1;

    processControlBlockADT process_pcb = mem_alloc(process_manager->memory_manager, sizeof(processControlBlockCDT));
    char *stack = mem_alloc(process_manager->memory_manager, PROCESS_STACK_SIZE);

    if (process_pcb == NULL || stack == NULL)
        return -1;

    pid_t pid;
    for (pid = INIT_PROCESS_PID + 1; process_manager->processes[pid] != NULL && pid < MAX_PROCESSES; pid++)
        ;

    process_pcb->pid = pid;
    process_pcb->stack = stack;
    process_pcb->parent_pid = parent_pid;
    process_pcb->status = READY;
    process_pcb->priority = LOW;
    process_pcb->rip = (uint64_t)start_wrapper;

    struct startFrame *start_frame = (startFrame *)(process_pcb->stack + PROCESS_STACK_SIZE - sizeof(startFrame));
    start_frame->process_manager = process_manager;
    start_frame->process_start = process_start;
    start_frame->pid = pid;
    start_frame->argv = argv;

    registers64_t *call_frame = (registers64_t *)(process_pcb->stack + PROCESS_STACK_SIZE - sizeof(startFrame) - sizeof(registers64_t));
    call_frame->rip = process_pcb->rip;
    call_frame->rsp = (uint64_t)(process_pcb->stack + PROCESS_STACK_SIZE);
    call_frame->ss = 0x0;
    call_frame->cs = 0x8;
    call_frame->rflags = 0x202;

    process_pcb->rsp = (uint64_t)(process_pcb->stack + PROCESS_STACK_SIZE - sizeof(startFrame) - sizeof(registers64_t));

    process_manager->num_processes += 1;

    return process_pcb->pid;
}

int exit_process(processManagerADT process_manager, pid_t pid, int64_t status)
{
    if (pid >= MAX_PROCESSES || process_manager->processes[pid] == NULL)
        return -1;

    process_manager->processes[pid]->status = EXITED;
    process_manager->processes[pid]->ret = status;
    mem_free(process_manager->memory_manager, process_manager->processes[pid]->stack);

    for (pid_t i = 0; i < MAX_PROCESSES; i++)
        if (process_manager->processes[i] != NULL && process_manager->processes[i]->parent_pid == pid)
            process_manager->processes[i]->parent_pid = INIT_PROCESS_PID;

    if (process_manager->processes[pid]->parent_pid == INIT_PROCESS_PID)
    {
        mem_free(process_manager->memory_manager, process_manager->processes[pid]);
        process_manager->processes[pid] = NULL;
        process_manager->num_processes -= 1;
    }

    go_to_scheduler();
    return 0;
}

int block_process(processManagerADT process_manager, pid_t pid)
{
    if (pid >= MAX_PROCESSES || process_manager->processes[pid] == NULL)
        return -1;

    process_manager->processes[pid]->status = BLOCKED;

    go_to_scheduler();
    return 0;
}

int unblock_process(processManagerADT process_manager, pid_t pid)
{
    if (pid >= MAX_PROCESSES || process_manager->processes[pid] == NULL)
        return -1;

    process_manager->processes[pid]->status = READY;
    return 0;
}

int kill_process(processManagerADT process_manager, pid_t pid) {
    if (pid >= MAX_PROCESSES || process_manager->processes[pid] == NULL)
        return -1;

    for (pid_t pid_i = 0; pid_i < MAX_PROCESSES; pid_i++)
        if (process_manager->processes[pid_i] != NULL && process_manager->processes[pid_i]->parent_pid == pid)
            kill_process(process_manager, pid_i);

    mem_free(process_manager->memory_manager, process_manager->processes[pid]->stack);
    mem_free(process_manager->memory_manager, process_manager->processes[pid]);
    process_manager->processes[pid] = NULL;
    process_manager->num_processes -= 1;

    go_to_scheduler();
    return 0;
}

processControlBlockADT* get_pcbs(processManagerADT process_manager) {
    return process_manager->processes;
}

uint64_t get_ps_data(){
    
}