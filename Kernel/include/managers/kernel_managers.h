#ifndef __KERNEL_MANAGERS_H__
#define __KERNEL_MANAGERS_H__

#include <managers/memoryManager.h>
#include <managers/processManager.h>
#include <managers/scheduler.h>

extern memoryManagerADT the_memory_manager;
extern processManagerADT the_process_manager;
extern schedulerADT the_scheduler;

#endif
