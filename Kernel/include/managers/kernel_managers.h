#ifndef __KERNEL_MANAGERS__H__
#define __KERNEL_MANAGERS__H__

#include <managers/semaphoreManager.h>
#include <managers/pipesManager.h>
#include <managers/scheduler.h>
#include <managers/processManager.h>
#include <managers/memoryManager.h>


extern memoryManagerADT the_memory_manager;
extern processManagerADT the_process_manager;
extern schedulerADT the_scheduler;
extern semaphoreManagerADT the_semaphore_manager;
extern pipesManagerADT the_pipes_manager;

#endif
