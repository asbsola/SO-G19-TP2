#include <managers/scheduler.h>

typedef int64_t pid_t;
struct schedulerCDT {
    processManagerADT process_manager;
    pid_t current;
    // Queue prioritie_queues[];
};

schedulerADT init_scheduler(processManagerADT process_manager, memoryManagerADT memory_manager){
    schedulerADT scheduler = mem_alloc(memory_manager, sizeof(struct schedulerCDT));

    scheduler->process_manager = process_manager;
    char* argv[] = {NULL};
    scheduler->current = create_process(scheduler->process_manager, -1, 0, idle, argv);

    return scheduler;
}

uint64_t context_switch(schedulerADT scheduler, uint64_t rsp){
    return get_pcbs(scheduler->process_manager)[scheduler->current]->rsp;
}