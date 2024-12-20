// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <tests.h>

#define TOTAL_PROCESSES 250

enum State { RUNNING_TEST, BLOCKED_TEST, KILLED_TEST };

typedef struct P_rq {
	pid_t pid;
	enum State state;
} p_rq;

uint64_t test_processes(char **argv, int argc) {
	uint8_t rq;
	uint8_t alive = 0;
	uint8_t action;
	uint64_t max_processes;
	uint64_t max_iters;
	char *argvAux[] = {"endless_loop", NULL};
	p_rq p_rqs[TOTAL_PROCESSES];

	uint64_t iter = 0;

	if (argc < 3) {
		puts_with_color(
			"test_processes: ERROR must provide max_iters and "
			"max_processes (tops at 250)\n",
			0xFF0000);
		return -1;
	}

	max_iters = satoi(argv[1]);
	if (max_iters <= 0) {
		puts_with_color("test_processes: ERROR max_iters must be greater than 0\n", 0xFF0000);
		return -1;
	}

	max_processes = satoi(argv[2]);
	if (max_processes <= 0) {
		puts_with_color("test_processes: ERROR max_processes must be greater than 0\n", 0xFF0000);
		return -1;
	}
	if (max_processes > TOTAL_PROCESSES) {
		puts_with_color("test_processes: ERROR max_processes must be less than 250\n", 0xFF0000);
		return -1;
	}

	uint8_t in_background = (argc >= 4 && argv[argc - 1][0] == '&');

	while (iter++ < max_iters) {
		if (!in_background) {
			puts_with_color("\n-------------------\n", 0xc2daff);
			puts_with_color("Testing processes -> ", 0xc2daff);
			printf("iter: %ld of %ld\n", (uint64_t)iter, (uint64_t)max_iters);
		}

		// Create max_processes processes
		for (rq = 0; rq < max_processes; rq++) {
			p_rqs[rq].pid = sys_create_process(endless_loop, argvAux, KEYBOARD_INPUT_FD, SCREEN_OUTPUT_FD);

			if (p_rqs[rq].pid == -1) {
				puts_with_color("test_processes: ERROR creating process\n", 0xFF0000);
				return -1;
			} else {
				p_rqs[rq].state = RUNNING_TEST;
				alive++;
			}
		}

		if (!in_background) printf("processes created: %ld\n", rq);

		// Randomly kills, blocks or unblocks processes until every one has been
		// killed
		while (alive > 0) {
			for (rq = 0; rq < max_processes; rq++) {
				action = GetUniform(100) % 2;

				switch (action) {
					case 0:
						if (p_rqs[rq].state == RUNNING_TEST || p_rqs[rq].state == BLOCKED_TEST) {
							if (sys_kill_process_by_pid(p_rqs[rq].pid, 0) == -1) {
								puts_with_color("test_processes: ERROR killing process\n", 0xFF0000);
								return -1;
							}
							p_rqs[rq].state = KILLED_TEST;
							alive--;
						}
						break;

					case 1:
						if (p_rqs[rq].state == RUNNING_TEST) {
							if (sys_block_process_by_pid(p_rqs[rq].pid) == -1) {
								puts_with_color("test_processes: ERROR blocking process\n", 0xFF0000);
								return -1;
							}
							p_rqs[rq].state = BLOCKED_TEST;
						}
						break;
				}
			}

			// Randomly unblocks processes
			for (rq = 0; rq < max_processes; rq++)
				if (p_rqs[rq].state == BLOCKED_TEST && GetUniform(100) % 2) {
					if (sys_unblock_process_by_pid(p_rqs[rq].pid) == -1) {
						puts_with_color("test_processes: ERROR unblocking process\n", 0xFF0000);
						return -1;
					}
					p_rqs[rq].state = RUNNING_TEST;
				}
		}
	}

	return 0;
}
