#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <managers/memoryManager.h>
#include <managers/processControlBlock.h>
#include <idle.h>
#include <stdint.h>

typedef struct schedulerCDT *schedulerADT;

schedulerADT init_scheduler(memoryManagerADT memory_manager);

int schedule_process(schedulerADT scheduler, processControlBlockADT process);
int deschedule_process(schedulerADT scheduler, processControlBlockADT process);
pid_t get_current_pid(schedulerADT scheduler);
int change_process_priority(schedulerADT scheduler, processControlBlockADT pid, processPriority old_priority, processPriority new_priority);


uint64_t context_switch(schedulerADT scheduler, uint64_t rsp);

#endif
