// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <tests.h>

#define MILLIS_PARAM "1000"

uint64_t test_idle_cleanup(char **argv, int argc) {
	uint64_t children_count = 0;
	if (argc < 2 || (children_count = atoi(argv[1])) <= 0) {
		puts_with_color("test_idle_cleanup: ERROR must provide max_processes (tops at 250)\n", 0xFF0000);
		return -1;
	}

	uint8_t in_background = (argc > 2 && argv[argc - 1][0] == '&');
	children_count = (children_count > TOTAL_PROCESSES) ? TOTAL_PROCESSES : children_count;

	char *argvAux[] = {"bussy_wait_proc", MILLIS_PARAM, NULL};

	if (!in_background) puts_with_color("Creating processes...\n", 0xFFFFFF);

	for (uint64_t i = 0; i < children_count; i++) { sys_create_process(bussy_wait_proc, argvAux, KEYBOARD_INPUT_FD, SCREEN_OUTPUT_FD); }

	if (!in_background) puts_with_color("Exiting (check ps too see children status)\n", 0xFFFFFF);

	return 0;
}
