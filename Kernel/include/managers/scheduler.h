#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <managers/processManager.h>
#include <idle.h>
#include <stdint.h>

typedef struct schedulerCDT* schedulerADT;

schedulerADT init_scheduler(processManagerADT process_manager, memoryManagerADT memory_manager);
uint64_t* context_switch(schedulerADT scheduler, uint64_t* rsp);

#endif
