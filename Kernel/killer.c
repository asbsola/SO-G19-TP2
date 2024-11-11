// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <killer.h>
#include <managers/kernel_managers.h>
#include <managers/processControlBlock.h>
#include <managers/semaphoreManager.h>

void free_pipe_mutex_recursive(pid_t my_pid);

void free_pipe_mutex(pid_t pid, uint64_t recursive) {
    if (recursive) free_pipe_mutex_recursive(pid);

    processControlBlockADT pcb = get_process(the_process_manager, pid);

    if (pcb->pipe_mutex != -1) sem_up(the_semaphore_manager, pcb->pipe_mutex);
}

void free_pipe_mutex_recursive(pid_t my_pid) {
    for (pid_t pid = 0; pid <= MAX_PROCESSES; pid++)
        if (is_child(the_process_manager, my_pid, pid)) free_pipe_mutex(pid, 1);
}


int kill_signal(pid_t pid, uint64_t recursive) {
    free_pipe_mutex(pid, recursive);
    return kill_process(the_process_manager, pid, recursive);
}

uint64_t killer(char **argv, int argc) {
	if (the_process_manager == NULL) return -1;
	int recursive = (argc > 1 && argv[1][0] == 'r');
    pid_t pid = get_fg_pid(the_process_manager);
    if (pid == -1) return -1;
	return kill_signal(pid, recursive);
}
