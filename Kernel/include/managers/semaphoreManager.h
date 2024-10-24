#ifndef __SEMAPHORE_MANAGER_H__
#define __SEMAPHORE_MANAGER_H__

#include <managers/memoryManager.h>
#include <def.h>

typedef struct semaphoreManagerCDT *semaphoreManagerADT;
typedef struct semaphoreCDT *semaphoreADT;

semaphoreManagerADT init_semaphore_manager(memoryManagerADT memory_manager);
sem_t open_sem(semaphoreManagerADT semaphore_manager, uint64_t value);
int close_sem(semaphoreManagerADT semaphore_manager, sem_t sem);
void up_sem(semaphoreManagerADT semaphore_manager, sem_t sem);
void down_sem(semaphoreManagerADT semaphore_manager, sem_t sem);

#endif