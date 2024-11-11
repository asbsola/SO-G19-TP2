// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <idle.h>
#include <managers/kernel_managers.h>

void remove_orphans(processManagerADT process_manager) {
	processControlBlockADT *processes = get_processes(process_manager);
	for (int i = get_max_pid(process_manager); i >= 0; i--) {
		if (processes[i] != NULL && processes[i]->parent_pid == IDLE_PROCESS_PID && processes[i]->status == EXITED) { remove_process(process_manager, processes[i]->pid); }
	}
}

uint64_t idle(char **argv, int argc) {
	char *screen_service_args[] = {"screen_service", NULL};
	pid_t screen_pid = create_process(the_process_manager, 0, screen_service, screen_service_args, SCREEN_OUTPUT_FD, -1);
	nice(the_process_manager, screen_pid, HIGH);

	char *shell_args[] = {"shell", NULL};
	create_process(the_process_manager, 0, (uint64_t(*)(char **, int))(SHELL_CODE_ADDRESS), shell_args, KEYBOARD_INPUT_FD, SCREEN_OUTPUT_FD);

	while (1) {
		remove_orphans(the_process_manager);
		yield();
	}
}
