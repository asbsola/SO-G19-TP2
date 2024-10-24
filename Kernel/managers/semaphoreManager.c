// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <managers/semaphoreManager.h>
#include <utils/list.h>
#include <utils/string.h>
#define MAX(a, b) ((a)<(b)?(b):(a))

struct semaphoreCDT {
    uint64_t value;
    char* name;
    ListADT waiting_processes;
};

struct semaphoreManagerCDT {
    sem_t last_sem;
    uint64_t num_semaphores;
    semaphoreADT semaphores[MAX_SEMAPHORES];
    memoryManagerADT memory_manager;
};

semaphoreManagerADT init_semaphore_manager(memoryManagerADT memory_manager){
    semaphoreManagerADT semaphore_manager = mem_alloc(memory_manager, sizeof(struct semaphoreManagerCDT));

    semaphore_manager->last_sem = -1;
    semaphore_manager->num_semaphores = 0;
    for(int i=0; i<MAX_SEMAPHORES; i++) semaphore_manager->semaphores[i] = NULL;
    semaphore_manager->memory_manager = memory_manager;

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
    semaphore->waiting_processes = list_init(semaphore_manager->memory_manager);

    semaphore_manager->last_sem = MAX(semaphore_manager->last_sem, sem);
    semaphore_manager->num_semaphores++;
    semaphore_manager->semaphores[sem] = semaphore;

    return sem;
}

sem_t open_sem_named(semaphoreManagerADT semaphore_manager, uint64_t value, char* name){
    if(get_sem_named(semaphore_manager, name) != -1) return -1;

    sem_t sem = open_sem(semaphore_manager, value);
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

int close_sem(semaphoreManagerADT semaphore_manager, sem_t sem){
    if(invalid_sem(semaphore_manager, sem)) return -1;

    semaphoreADT semaphore = semaphore_manager->semaphores[sem];
    // unblock all semaphore->waiting_processes
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
}

int close_sem_named(semaphoreManagerADT semaphore_manager, char* name){
    sem_t sem = get_sem_named(semaphore_manager, name);
    return close_sem(semaphore_manager, sem);
}

int up_sem(semaphoreManagerADT semaphore_manager, sem_t sem){
    if(invalid_sem(semaphore_manager, sem)) return -1;

}

int up_sem_named(semaphoreManagerADT semaphore_manager, char* name){
    sem_t sem = get_sem_named(semaphore_manager, name);
    return up_sem(semaphore_manager, sem);
}

int down_sem(semaphoreManagerADT semaphore_manager, sem_t sem){
    if(invalid_sem(semaphore_manager, sem)) return -1;

}

int down_sem_named(semaphoreManagerADT semaphore_manager, char* name){
    sem_t sem = get_sem_named(semaphore_manager, name);
    return down_sem(semaphore_manager, sem);
}