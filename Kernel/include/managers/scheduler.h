#ifndef __SCHEDULER__H__
#define __SCHEDULER__H__



#include <managers/processControlBlock.h>
#include <stdint.h>
#include <registers.h>
#include <utils/list.h>
#include <utils/utils.h>
#include <def.h>
#include <drivers/videoDriver.h>
#include <lib.h>
#include <managers/memoryManager.h>


typedef struct schedulerCDT *schedulerADT;

schedulerADT init_scheduler(memoryManagerADT memory_manager);

int schedule_process(schedulerADT scheduler, processControlBlockADT process);
int deschedule_process(schedulerADT scheduler, processControlBlockADT process);
pid_t get_current_pid(schedulerADT scheduler);
int change_process_priority(schedulerADT scheduler, processControlBlockADT pid,
                            processPriority old_priority,
                            processPriority new_priority);

uint64_t context_switch(schedulerADT scheduler, uint64_t rsp);

#endif
