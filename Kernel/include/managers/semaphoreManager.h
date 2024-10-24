#ifndef __SEMAPHORE_MANAGER_H__
#define __SEMAPHORE_MANAGER_H__

#include <managers/scheduler.h>
#include <managers/processManager.h>
#include <managers/memoryManager.h>
#include <def.h>

typedef struct semaphoreManagerCDT *semaphoreManagerADT;
typedef struct semaphoreCDT *semaphoreADT;

semaphoreManagerADT init_semaphore_manager(memoryManagerADT memory_manager, processManagerADT process_manager, schedulerADT scheduler);
sem_t open_sem(semaphoreManagerADT semaphore_manager, uint64_t value);
int close_sem(semaphoreManagerADT semaphore_manager, sem_t sem);
int up_sem(semaphoreManagerADT semaphore_manager, sem_t sem);
int down_sem(semaphoreManagerADT semaphore_manager, sem_t sem);

sem_t open_sem_named(semaphoreManagerADT semaphore_manager, uint64_t value, char* name);
int close_sem_named(semaphoreManagerADT semaphore_manager, char* name);
int up_sem_named(semaphoreManagerADT semaphore_manager, char* name);
int down_sem_named(semaphoreManagerADT semaphore_manager, char* name);

#endif
