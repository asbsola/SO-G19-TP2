#include <lib.h>
#include <registers.h>
#include <managers/scheduler.h>
#include <drivers/videoDriver.h>

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

        return get_pcbs(scheduler->process_manager)[scheduler->current]->rsp;
    }

    /*
    processControlBlockADT current = get_pcbs(scheduler->process_manager)[scheduler->current];
    uint64_t rsp_aux = current->rsp;

    //print_register("rsp: ", 5, rsp_aux, 0x00ffffff); 
    //print_register("rsp2: ", 6, rsp, 0x00ffffff); 
    
    current->rsp = rsp;
    */

    return rsp;//current->rsp;
}
