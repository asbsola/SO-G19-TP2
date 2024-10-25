// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <managers/semaphoreManager.h>
#include <managers/processControlBlock.h>
#include <utils/list.h>
#include <utils/string.h>
#define MAX(a, b) ((a)<(b)?(b):(a))

struct semaphoreCDT {
    uint64_t value;
    uint8_t lock;
    char* name;
    ListADT waiting_processes;
};

struct semaphoreManagerCDT {
    sem_t last_sem;
    uint64_t num_semaphores;
    semaphoreADT semaphores[MAX_SEMAPHORES];
    memoryManagerADT memory_manager;
    processManagerADT process_manager;
    schedulerADT scheduler;
};

void acquire(uint8_t* lock);
void release(uint8_t* lock);

semaphoreManagerADT init_semaphore_manager(memoryManagerADT memory_manager, processManagerADT process_manager, schedulerADT scheduler){
    semaphoreManagerADT semaphore_manager = mem_alloc(memory_manager, sizeof(struct semaphoreManagerCDT));

    semaphore_manager->last_sem = -1;
    semaphore_manager->num_semaphores = 0;
    for(int i=0; i<MAX_SEMAPHORES; i++) semaphore_manager->semaphores[i] = NULL;
    semaphore_manager->memory_manager = memory_manager;
    semaphore_manager->process_manager = process_manager;
    semaphore_manager->scheduler = scheduler;

    return semaphore_manager;
}

int invalid_sem(semaphoreManagerADT semaphore_manager, sem_t sem){
    return sem < 0 || sem >= MAX_SEMAPHORES || semaphore_manager->semaphores[sem] == NULL;
}

sem_t get_sem_named(semaphoreManagerADT semaphore_manager, char* name){
    sem_t sem;
    for(sem = 0; sem <= semaphore_manager->last_sem; sem++){
        if(!invalid_sem(semaphore_manager, sem) && str_cmp(semaphore_manager->semaphores[sem]->name, name) == 0)
            return sem;
    }
    return -1;
}

sem_t open_sem(semaphoreManagerADT semaphore_manager, uint64_t value){
    if(semaphore_manager->num_semaphores >= MAX_SEMAPHORES) return -1;

    sem_t sem;
    for(sem = 0; sem < MAX_SEMAPHORES && semaphore_manager->semaphores[sem] != NULL; sem++);
    
    semaphoreADT semaphore = mem_alloc(semaphore_manager->memory_manager, sizeof(struct semaphoreCDT));
    if(semaphore == NULL) return -1;

    semaphore->value = value;
    semaphore->name = NULL;
    semaphore->lock = 1;
    semaphore->waiting_processes = list_init(semaphore_manager->memory_manager);

    semaphore_manager->last_sem = MAX(semaphore_manager->last_sem, sem);
    semaphore_manager->num_semaphores++;
    semaphore_manager->semaphores[sem] = semaphore;

    return sem;
}

sem_t open_sem_named(semaphoreManagerADT semaphore_manager, uint64_t value, char* name){
    sem_t sem = get_sem_named(semaphore_manager, name);
    
    if(sem != -1) return sem;

    sem = open_sem(semaphore_manager, value);
    if(sem == -1) return -1;

    semaphoreADT semaphore = semaphore_manager->semaphores[sem];
    int len = str_len(name);
    semaphore->name = mem_alloc(semaphore_manager->memory_manager, (len + 1) * sizeof(char));
    if(semaphore->name == NULL){
        close_sem(semaphore_manager, sem);
        return -1;
    }
    str_cpy(semaphore->name, name);

    return sem;
}

void unblock_waiting(semaphoreManagerADT semaphore_manager, semaphoreADT semADT) {
    while (!list_is_empty(semADT->waiting_processes)) {
        processControlBlockADT processADT = list_next(semADT->waiting_processes);
        unblock_process(semaphore_manager->process_manager, processADT->pid);
        list_remove(semADT->waiting_processes, processADT);
    }
}

int close_sem(semaphoreManagerADT semaphore_manager, sem_t sem){
    if(invalid_sem(semaphore_manager, sem)) return -1;
    semaphoreADT semaphore = semaphore_manager->semaphores[sem];

    acquire(&semaphore->lock);
    unblock_waiting(semaphore_manager, semaphore);

    free_list(semaphore->waiting_processes);
    mem_free(semaphore_manager->memory_manager, semaphore->name);
    mem_free(semaphore_manager->memory_manager, semaphore);
    semaphore_manager->semaphores[sem] = NULL;
    semaphore_manager->num_semaphores--;
    if(semaphore_manager->last_sem == sem){
        sem_t i;
        for(i=sem-1; i>=0 && semaphore_manager->semaphores[i] == NULL; i--);
        semaphore_manager->last_sem = i;
    }

    return 0;
}

int close_sem_named(semaphoreManagerADT semaphore_manager, char* name){
    sem_t sem = get_sem_named(semaphore_manager, name);
    return close_sem(semaphore_manager, sem);
}

int up_sem(semaphoreManagerADT semaphore_manager, sem_t sem){
    if(invalid_sem(semaphore_manager, sem)) return -1;
    semaphoreADT semADT = semaphore_manager->semaphores[sem];

    acquire(&semADT->lock);

    semADT->value++;

    if (!list_is_empty(semADT->waiting_processes)) {
        processControlBlockADT processADT = list_next(semADT->waiting_processes);
        unblock_process(semaphore_manager->process_manager, processADT->pid);
        list_remove(semADT->waiting_processes, processADT);
    }

    release(&semADT->lock);
    return 0;
}

int up_sem_named(semaphoreManagerADT semaphore_manager, char* name){
    sem_t sem = get_sem_named(semaphore_manager, name);
    return up_sem(semaphore_manager, sem);
}

int down_sem(semaphoreManagerADT semaphore_manager, sem_t sem){
    if(invalid_sem(semaphore_manager, sem)) return -1;
    semaphoreADT semADT = semaphore_manager->semaphores[sem];

    acquire(&semADT->lock);

    if (semADT->value == 0) {
        pid_t current = get_current_process(semaphore_manager->scheduler);
        list_add(semADT->waiting_processes, get_process(semaphore_manager->process_manager, current));
        block_process(semaphore_manager->process_manager, current);
    }

    semADT->value--;

    release(&semADT->lock);

    return 0;
}

int down_sem_named(semaphoreManagerADT semaphore_manager, char* name){
    sem_t sem = get_sem_named(semaphore_manager, name);
    return down_sem(semaphore_manager, sem);
}
