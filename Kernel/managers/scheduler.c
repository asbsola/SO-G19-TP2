#include <lib.h>
#include <registers.h>
#include <managers/scheduler.h>
#include <drivers/videoDriver.h>
#include <utils/list.h>

struct schedulerCDT {
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

    return scheduler;
}

int schedule_process(schedulerADT scheduler, processControlBlockADT process){
    return list_add(scheduler->process_list, process);
}

processControlBlockADT next_process(schedulerADT scheduler){
    return (processControlBlockADT) list_next(scheduler->process_list);
}

uint64_t context_switch(schedulerADT scheduler, uint64_t rsp){
    if (scheduler->current_process != NULL){
        scheduler->current_process->rsp = rsp;
    }

    scheduler->current_process = next_process(scheduler);
    return scheduler->current_process->rsp;
}