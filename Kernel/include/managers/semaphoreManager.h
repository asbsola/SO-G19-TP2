#ifndef __SEMAPHORE_MANAGER__H__
#define __SEMAPHORE_MANAGER__H__

#include <def.h>
#include <managers/memoryManager.h>
#include <managers/processManager.h>
#include <managers/scheduler.h>
#include <utils/list.h>
#include <utils/string.h>

typedef struct processManagerCDT *processManagerADT;
typedef struct semaphoreManagerCDT *semaphoreManagerADT;

semaphoreManagerADT init_semaphore_manager(memoryManagerADT memory_manager, schedulerADT scheduler, processManagerADT process_manager);
sem_t sem_open(semaphoreManagerADT semaphore_manager, uint64_t value);
int sem_close(semaphoreManagerADT semaphore_manager, sem_t sem);
int sem_up(semaphoreManagerADT semaphore_manager, sem_t sem);
int sem_down(semaphoreManagerADT semaphore_manager, sem_t sem);

sem_t sem_open_named(semaphoreManagerADT semaphore_manager, uint64_t value, char *name);
int sem_close_named(semaphoreManagerADT semaphore_manager, char *name);

#endif
