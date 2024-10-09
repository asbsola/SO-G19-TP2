#include <lib.h>
#include <registers.h>
#include <managers/scheduler.h>
#include <drivers/videoDriver.h>

typedef int64_t pid_t;
struct schedulerCDT {
    processManagerADT process_manager;
    pid_t current;
    // Queue prioritie_queues[];
};

schedulerADT init_scheduler(processManagerADT process_manager, memoryManagerADT memory_manager){
    schedulerADT scheduler = mem_alloc(memory_manager, sizeof(struct schedulerCDT));

    if (scheduler == NULL) return NULL;

    scheduler->process_manager = process_manager;
    char* argv[] = {NULL};
    scheduler->current = -1;

    create_process(scheduler->process_manager, -1, 0, idle, argv);
    //create_process(scheduler->process_manager, -1, 0, idle, argv);

    return scheduler;
}

uint64_t context_switch(schedulerADT scheduler, uint64_t rsp){
    if (scheduler->current == -1) {
        scheduler->current++;        
        processControlBlockADT current = get_pcbs(scheduler->process_manager)[scheduler->current];

        return current->rsp;
    }

    processControlBlockADT current = get_pcbs(scheduler->process_manager)[scheduler->current];
    uint64_t rsp_aux = current->rsp;
    
    current->rsp = rsp;

    return rsp_aux;
}
