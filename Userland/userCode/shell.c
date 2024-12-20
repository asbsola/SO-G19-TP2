// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <shell.h>

ModuleDescriptor modules[] = {
	{"help", "displays available modules", BUILT_IN, help}, 
    {"clear", "clears the screens text buffer", BUILT_IN, cls}, 
    {"sysinfo", "displays system information", PROCESS, info}, 
    {"fontsize", "change font size", BUILT_IN, font_size}, 
    {"time", "display current time", PROCESS, time}, 
    {"regs", "displays captured registers (ESC key to capture)", BUILT_IN, regs}, 
    {"beep", "beeps", BUILT_IN, beep}, 
    {"song", "plays a short tune while displaying graphics", PROCESS, cucaracha}, 
    {"calculator", "positive integer calculator", PROCESS, calculator}, 
    {"eliminator", "eliminator game", PROCESS, eliminator}, 
    {"jump", "jumps to address given by user in decimal (12345 causes invalid opcode)", PROCESS, jump}, 
    {"test_mm", "tests the memory manager", PROCESS, test_mm}, 
    {"test_processes", "tests processes", PROCESS, test_processes}, 
    {"test_priority", "tests priority", PROCESS, test_prio}, 
    {"test_sync", "tests syncro", PROCESS, test_sync}, 
    {"test_wait", "tests waits and return values", PROCESS, test_wait}, 
    {"test_priority_dist", "tests priority distribution", PROCESS, test_prio_dist}, 
    {"test_idle_cleanup", "tests idle cleanup of orphans", PROCESS, test_idle_cleanup}, 
    {"test_pipes", "tests pipes", PROCESS, test_pipes}, 
    {"mem", "displays memory status", PROCESS, mem}, 
    {"ps", "displays information about processes", PROCESS, ps}, 
    {"nice", "changes process priority by PID", BUILT_IN, nice}, 
    {"kill", "terminates a process by its PID. If the flag 'r' is provided, it also terminates all its descendants", BUILT_IN, kill}, 
    {"cleanup", "removes all exited processes", BUILT_IN, cleanup}, 
    {"block", "blocks or unblocks a procces by a PID", BUILT_IN, block}, 
    {"loop", "prints its PID every n seconds", PROCESS, loop}, 
    {"phylo", "the dining philosophers problem", PROCESS, phylo}, 
    {"cat", "print on the standard output", PROCESS, cat}, 
    {"wc", "counts the number of lines in the input", PROCESS, wc}, 
    {"filter", "Filters the input vowels", PROCESS, filter}, 
    {"echo", "echos the input", PROCESS, echo},
    {"sh", "a shell (inside a shell :O)", PROCESS, (uint64_t(*)(char **, int))(SHELL_CODE_ADDRESS)},
};

static int current_font_size = 1;

void run_shell() {
	char shell_input[MAX_SHELL_INPUT];
	shell_input[0] = 0;
	Command commands[MAX_COMMANDS];
	pid_t pids[MAX_COMMANDS];
	fd_t pipes[MAX_COMMANDS - 1];
	int num_cmds = 0;
	sys_nice(sys_get_pid(), HIGH);

	while (1) {
		sys_set_font_size(current_font_size);

		puts_with_color("shell> ", 0x006fb5fb);
		int len = getline(shell_input, MAX_SHELL_INPUT);
        shell_input[len - 1] = '\0';

		if (strcmp(shell_input, "exit") == 0 || (len <= 1 && sys_get_stdin() != KEYBOARD_INPUT_FD)) break;
		if (len == 1) continue;

		int64_t ans = get_commands(shell_input, commands, &num_cmds);
		if (ans == -1) continue;

		if (num_cmds == 1) {
			pids[0] = run_cmd(commands[0], sys_get_stdin(), sys_get_stdout(), num_cmds);
		} else {
			int i;
			for (i = 0; i < num_cmds - 1; i++) {
				pipes[i] = sys_pipe_open(NON_CANNONICAL);

				if (i == 0) {
					pids[i] = run_cmd(commands[i], sys_get_stdin(), pipes[i], num_cmds);
				} else {
					pids[i] = run_cmd(commands[i], pipes[i - 1], pipes[i], num_cmds);
				}
			}
			pids[i] = run_cmd(commands[num_cmds - 1], pipes[num_cmds - 2], sys_get_stdout(), num_cmds);
		}
		for (int i = 0; i < num_cmds; i++) {
			if (commands[i].argv[commands[i].argc - 1][0] != '&') { sys_wait_pid(pids[i], &ans); }
			if (i < num_cmds - 1) { sys_pipe_send_eof(pipes[i]); }
		}

		for (int i = 0; i < num_cmds - 1; i++) {
			sys_pipe_close(pipes[i]);
			free_args(commands[i].argv);
		}
		free_args(commands[num_cmds - 1].argv);
	}
}

int get_module_index(char *module_name) {
	for (uint32_t i = 0; i < sizeof(modules) / sizeof(modules[0]); i++) {
		if (strcmp(module_name, modules[i].module_name) == 0) { return i; }
	}
	return -1;
}

void free_commands_argv(Command *commands, int num_cmds) {
	for (int i = 0; i < num_cmds; i++) free_args(commands[i].argv);
}

int get_commands(char *shell_input, Command *commands, int *num_cmds) {
	char **cmds = split(shell_input, num_cmds, '|');
	for (int i = 0; i < *num_cmds; i++) { trim(cmds[i]); }
	for (int i = 0; i < *num_cmds; i++) {
		commands[i].argv = split(cmds[i], &commands[i].argc, ' ');

		for (int j = 0; j < commands[i].argc; j++) { trim(commands[i].argv[j]); }

		int module_index = get_module_index(commands[i].argv[0]);
		if (module_index == -1) {
			printf("Unknown command: %s\n", commands[i].argv[0]);
			free_args(cmds);
			free_commands_argv(commands, i + 1);
			return -1;
		}
		commands[i].module = modules[module_index];
	}

	free_args(cmds);
	return 0;
}

uint64_t run_cmd(Command cmd, fd_t stdin, fd_t stdout, int num_cmds) {
	pid_t pid = -1;

	if (cmd.module.module_type == PROCESS) {
		pid = sys_create_process(cmd.module.module, cmd.argv, stdin, stdout);
		if (pid == -1) {
			puts_with_color("Error creating process\n", 0xFF0000);
			return -1;
		}
	} else if (cmd.module.module_type == BUILT_IN && num_cmds == 1) {
		cmd.module.module(cmd.argv, cmd.argc);
    } else {
        pid = sys_create_process(run_built_in_subshell, cmd.argv, stdin, stdout);
		if (pid == -1) {
			puts_with_color("Error creating subshell\n", 0xFF0000);
			return -1;
		}
    }


	return pid;
}

uint64_t run_built_in_subshell(char** argv, int argc) {
    int module_index = get_module_index(argv[0]);
    return modules[module_index].module(argv, argc);
}

void free_args(char **args) {
	for (uint64_t i = 0; args[i] != NULL; i++) sys_free(args[i]);

	sys_free(args);
}

uint64_t help(char **argv, int argc) {
	puts_with_color(
		"arguments are separated by ' ' (spaces).\nadd & at the last argument of "
		"a command to run in background (only for processes).\nuse | to pipe commands\n\n",
		0xc2daff);

	puts("available modules:\n");

	puts("\t- exit: exit to kernel.\n");

	for (uint32_t i = 0; i < sizeof(modules) / sizeof(modules[0]); i++) {
		printf("\t- %s - ", (modules[i].module_type == BUILT_IN) ? "built-in" : "process ");
		puts_with_color(modules[i].module_name, 0x006fb5fb);
		printf(": %s.\n", modules[i].module_description);
	}

	putchar('\n');

	return 0;
}

uint64_t cls(char **argv, int argc) {
	sys_clear_text_buffer();

	return 0;
}

uint64_t font_size(char **argv, int argc) {
	int n = 0;
	printf("Choose font size (1-5) or 0 to exit: ");
	scan_line("%d", &n);

	while (n < 0 || n >= 6) {
		printf("Choose a valid font size (1-5) or 0 to exit: ");
		scan_line("%d", &n);
	}

	if (n != 0) {
		current_font_size = n;
		printf("Font size set to %d\n", n);
	}

	return 0;
}

uint64_t beep(char **argv, int argc) {
	sys_beep(500, 100);
	sys_beep(2000, 100);

	return 0;
}

uint64_t nice(char **argv, int argc) {
	if (argc < 3) {
		puts_with_color("nice: ERROR must provide pid and priority\n", 0xFF0000);
		return -1;
	}

	uint64_t pid = atoi(argv[1]);
	uint64_t priority;

	if (strcmp(argv[2], "LOW") == 0)
		priority = 0;
	else if (strcmp(argv[2], "MEDIUM") == 0)
		priority = 1;
	else if (strcmp(argv[2], "HIGH") == 0)
		priority = 2;
	else {
		puts_with_color("nice: ERROR priority must be LOW, MEDIUM or HIGH\n", 0xFF0000);
		return -1;
	}

	if (sys_nice(pid, priority) == -1) {
		puts("nice: ERROR changing priority\n");
		return -1;
	}

	return 0;
}

uint64_t kill(char **argv, int argc) {
	if (argc < 2) {
		puts_with_color("kill: ERROR must provide pid\n", 0xFF0000);
		return -1;
	}

	uint64_t kill_recursive = 0;

	if (argv[2][0] == 'r') { kill_recursive = 1; }

	int pid = atoi(argv[1]);

	if (sys_kill_process_by_pid(pid, kill_recursive) == -1) {
		puts_with_color("kill: ERROR could not kill process\n", 0xFF0000);
		return -1;
	}

	return 0;
}

uint64_t cleanup(char **argv, int argc) {
	int64_t ret;
	while (sys_wait(&ret) != -1);
	return 0;
}

uint64_t block(char **argv, int argc) {
	if (argc < 2) {
		puts_with_color("block: ERROR must provide pid\n", 0xFF0000);
		return -1;
	}
	int pid = atoi(argv[1]);

	processStatus status = sys_get_process_status(pid);

	if(status == BLOCKED) sys_unblock_process_by_pid(pid);
	
	if(status == READY || status == RUNNING) sys_block_process_by_pid(pid);

	return 0;
}
