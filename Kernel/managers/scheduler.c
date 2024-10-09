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

    return scheduler;
}

pid_t next_process(schedulerADT scheduler){
    return (scheduler->current + 1) % get_num_processes(scheduler->process_manager);
}

uint64_t context_switch(schedulerADT scheduler, uint64_t rsp){
    processControlBlockADT* processes = get_pcbs(scheduler->process_manager);
    if (scheduler->current == -1) {
        scheduler->current++;
        return processes[scheduler->current]->rsp;
    }

    processes[scheduler->current]->rsp = rsp;
    scheduler->current = next_process(scheduler);
    return processes[scheduler->current]->rsp;
}