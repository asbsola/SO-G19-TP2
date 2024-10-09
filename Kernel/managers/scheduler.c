#include <lib.h>
#include <registers.h>
#include <managers/scheduler.h>
#include <drivers/videoDriver.h>
#include <utils/list.h>

struct schedulerCDT {
    memoryManagerADT memory_manager;
    ListADT process_list;
    processControlBlockADT current_process;
};

schedulerADT init_scheduler(memoryManagerADT memory_manager){
    schedulerADT scheduler = mem_alloc(memory_manager, sizeof(struct schedulerCDT));
    if (scheduler == NULL) return NULL;

    ListADT process_list = list_init(memory_manager);
    if(process_list == NULL) return NULL;

    scheduler->process_list = process_list;
    scheduler->current_process = NULL;
    scheduler->memory_manager = memory_manager;

    return scheduler;
}

pid_t get_current_process(schedulerADT scheduler){
    if(scheduler->current_process == NULL) return -1;
    return scheduler->current_process->pid;
}

int schedule_process(schedulerADT scheduler, processControlBlockADT process){
    return list_add(scheduler->process_list, process);
}

int deschedule_process(schedulerADT scheduler, processControlBlockADT process){
    return list_remove(scheduler->process_list, process);
}

processControlBlockADT next_process(schedulerADT scheduler){
    return (processControlBlockADT) list_next(scheduler->process_list);
}

void handle_status(schedulerADT scheduler, processControlBlockADT process){
    switch (process->status) {
    case RUNNING:
        process->status = READY;
        break;
    case BLOCKED:
        list_remove(scheduler->process_list, process);
        break;
    case EXITED:
        list_remove(scheduler->process_list, process);
        break;
    case KILLED:
        list_remove(scheduler->process_list, process);
        mem_free(scheduler->memory_manager, process);
    default:
        break;
    }
}

uint64_t context_switch(schedulerADT scheduler, uint64_t rsp){
    if (scheduler->current_process == NULL){
        scheduler->current_process = next_process(scheduler);
        return scheduler->current_process->rsp;
    }

    scheduler->current_process->rsp = rsp;
    handle_status(scheduler, scheduler->current_process);

    scheduler->current_process = next_process(scheduler);
    scheduler->current_process->status = RUNNING;
    return scheduler->current_process->rsp;
}