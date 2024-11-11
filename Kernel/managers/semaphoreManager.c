// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <managers/semaphoreManager.h>

struct semaphoreManagerCDT {
	sem_t last_sem;
	uint64_t num_semaphores;
	semaphoreADT semaphores[MAX_SEMAPHORES];
	memoryManagerADT memory_manager;
	processManagerADT process_manager;
	schedulerADT scheduler;

	uint8_t named_sem_lock;
	uint8_t sem_lock;
};

void acquire(uint8_t *lock);
void release(uint8_t *lock);

semaphoreManagerADT init_semaphore_manager(memoryManagerADT memory_manager, schedulerADT scheduler, processManagerADT process_manager) {
	semaphoreManagerADT semaphore_manager = mem_alloc(memory_manager, sizeof(struct semaphoreManagerCDT));

	semaphore_manager->last_sem = -1;
	semaphore_manager->num_semaphores = 0;
	for (int i = 0; i < MAX_SEMAPHORES; i++) semaphore_manager->semaphores[i] = NULL;
	semaphore_manager->sem_lock = 1;
	semaphore_manager->named_sem_lock = 1;
	semaphore_manager->memory_manager = memory_manager;
	semaphore_manager->process_manager = process_manager;
	semaphore_manager->scheduler = scheduler;

	return semaphore_manager;
}

int invalid_sem(semaphoreManagerADT semaphore_manager, sem_t sem) {
	return sem < 0 || sem >= MAX_SEMAPHORES || semaphore_manager->semaphores[sem] == NULL;
}

sem_t get_sem_named(semaphoreManagerADT semaphore_manager, char *name) {
	sem_t sem;
	for (sem = 0; sem <= semaphore_manager->last_sem; sem++) {
		if (!invalid_sem(semaphore_manager, sem) && str_cmp(semaphore_manager->semaphores[sem]->name, name) == 0) return sem;
	}
	return -1;
}

sem_t sem_open(semaphoreManagerADT semaphore_manager, uint64_t value) {
	if (semaphore_manager->num_semaphores >= MAX_SEMAPHORES) return -1;
	acquire(&semaphore_manager->sem_lock);

	sem_t sem;
	for (sem = 0; sem < MAX_SEMAPHORES && semaphore_manager->semaphores[sem] != NULL; sem++);

	semaphoreADT semaphore = mem_alloc(semaphore_manager->memory_manager, sizeof(struct semaphoreCDT));
	if (semaphore == NULL) {
		release(&semaphore_manager->sem_lock);
		return -1;
	}

	semaphore->value = value;
	semaphore->name = NULL;
	semaphore->lock = 1;
	semaphore->waiting_processes = list_init(semaphore_manager->memory_manager);
	if (semaphore->waiting_processes == NULL) {
		mem_free(semaphore_manager->memory_manager, semaphore);
		release(&semaphore_manager->sem_lock);
		return -1;
	}
	semaphore_manager->last_sem = MAX(semaphore_manager->last_sem, sem);
	semaphore_manager->num_semaphores++;
	semaphore_manager->semaphores[sem] = semaphore;

	release(&semaphore_manager->sem_lock);
	return sem;
}

sem_t sem_open_named(semaphoreManagerADT semaphore_manager, uint64_t value, char *name) {
	acquire(&semaphore_manager->named_sem_lock);
	sem_t sem = get_sem_named(semaphore_manager, name);

	if (sem != -1) {
		release(&semaphore_manager->named_sem_lock);
		return sem;
	}

	sem = sem_open(semaphore_manager, value);
	if (sem == -1) {
		release(&semaphore_manager->named_sem_lock);
		return -1;
	}

	semaphoreADT semaphore = semaphore_manager->semaphores[sem];
	int len = str_len(name);
	semaphore->name = mem_alloc(semaphore_manager->memory_manager, (len + 1) * sizeof(char));
	if (semaphore->name == NULL) {
		release(&semaphore_manager->named_sem_lock);
		sem_close(semaphore_manager, sem);
		return -1;
	}
	str_cpy(semaphore->name, name);

	release(&semaphore_manager->named_sem_lock);
	return sem;
}

void unblock_waiting(semaphoreManagerADT semaphore_manager, semaphoreADT semADT) {
	while (!list_is_empty(semADT->waiting_processes)) {
		processControlBlockADT processADT = list_next(semADT->waiting_processes);
		unblock_process(semaphore_manager->process_manager, processADT->pid);
		processADT->blocking_sem = NULL;
		list_remove(semADT->waiting_processes, processADT);
	}
}

int sem_close(semaphoreManagerADT semaphore_manager, sem_t sem) {
	if (invalid_sem(semaphore_manager, sem)) return -1;
	semaphoreADT semaphore = semaphore_manager->semaphores[sem];

	acquire(&semaphore->lock);
	unblock_waiting(semaphore_manager, semaphore);

	free_list(semaphore->waiting_processes);
	mem_free(semaphore_manager->memory_manager, semaphore->name);
	mem_free(semaphore_manager->memory_manager, semaphore);
	semaphore_manager->semaphores[sem] = NULL;
	semaphore_manager->num_semaphores--;
	if (semaphore_manager->last_sem == sem) {
		sem_t i;
		for (i = sem - 1; i >= 0 && semaphore_manager->semaphores[i] == NULL; i--);
		semaphore_manager->last_sem = i;
	}

	return 0;
}

int sem_close_named(semaphoreManagerADT semaphore_manager, char *name) {
	sem_t sem = get_sem_named(semaphore_manager, name);
	return sem_close(semaphore_manager, sem);
}

int sem_up(semaphoreManagerADT semaphore_manager, sem_t sem) {
	if (invalid_sem(semaphore_manager, sem)) return -1;
	semaphoreADT semADT = semaphore_manager->semaphores[sem];

	acquire(&semADT->lock);

	if (!list_is_empty(semADT->waiting_processes)) {
		processControlBlockADT processADT = list_next(semADT->waiting_processes);
		unblock_process(semaphore_manager->process_manager, processADT->pid);
		list_remove(semADT->waiting_processes, processADT);
	} else
		semADT->value++;

	release(&semADT->lock);
	return 0;
}

int sem_down(semaphoreManagerADT semaphore_manager, sem_t sem) {
	if (invalid_sem(semaphore_manager, sem)) return -1;
	semaphoreADT semADT = semaphore_manager->semaphores[sem];

	acquire(&semADT->lock);

	if (semADT->value == 0) {
		pid_t current_pid = get_current_pid(semaphore_manager->scheduler);
		processControlBlockADT current_process = get_process(semaphore_manager->process_manager, current_pid);
		list_add(semADT->waiting_processes, current_process);
		current_process->blocking_sem = semADT;

		release(&semADT->lock);
		block_process(semaphore_manager->process_manager, current_pid);
		return 0;
	}

	semADT->value--;

	release(&semADT->lock);
	return 0;
}
