// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <std.h>
#include <stddef.h>
#include <stdint.h>
#include <syscall_adapters.h>
#include <test_utils.h>

#define SEM_NAME "sem"

int64_t global;	 // shared memory

void slowInc(int64_t *p, int64_t inc) {
	uint64_t aux = *p;
	sys_yield();  // This makes the race condition highly probable
	aux += inc;
	*p = aux;
}

uint64_t process_inc(char **argv, int argc) {
	uint64_t n;
	int64_t inc;
	int64_t use_sem;
	sem_t sem_id;

	if (argc != 4) return -1;

	if ((n = satoi(argv[1])) <= 0) {
		puts_with_color("test_sync: ERROR error max_iters must be greater than 0\n", 0xFF0000);
		return -1;
	}
	if ((inc = satoi(argv[2])) == 0) return -1;
	if ((use_sem = satoi(argv[3])) < 0) {
		puts_with_color("test_sync: ERROR error max_iters must be greater than 0\n", 0xFF0000);
		return -1;
	}

	if (use_sem) {
		sem_id = sys_sem_open_named(SEM_NAME, 1);
		if (sem_id == -1) {
			puts_with_color("test_sync: ERROR opening semaphore\n", 0xFF0000);
			return -1;
		}
	}

	uint64_t i;
	for (i = 0; i < n; i++) {
		if (use_sem) sys_sem_down(sem_id);
		slowInc(&global, inc);
		if (use_sem) sys_sem_up(sem_id);
	}

	return 0;
}

uint64_t test_sync(char **argv, int argc) {
	uint64_t pids[MAX_PROCESSES];

	if (argc < 4) {
		puts_with_color(
			"test_sync: ERROR must provide max_iters, max_pair_processes and "
			"use_syncro (0 is no syncro - 1 is syncro).\n",
			0xFF0000);
		return -1;
	}

	int process_count = satoi(argv[2]);

	if (process_count < 0 || process_count > (MAX_PROCESSES / 2)) {
		puts_with_color(
			"test_sync: ERROR max_pair_processes must be greater than "
			"0 and lower than 125\n",
			0xFF0000);
		return -1;
	}

	uint8_t in_background = (argc > 4 && argv[4][0] == '&');

	if (!in_background) printf("Starting test_sync\n");

	char *argvDec[] = {"process_inc", argv[1], "-1", argv[3], NULL};
	char *argvInc[] = {"process_inc", argv[1], "1", argv[3], NULL};

	global = 0;

	uint64_t i;
	for (i = 0; i < process_count; i++) {
		pids[i] = sys_create_process(process_inc, argvDec, KEYBOARD_INPUT_FD, SCREEN_OUTPUT_FD);
		pids[i + process_count] = sys_create_process(process_inc, argvInc, KEYBOARD_INPUT_FD, SCREEN_OUTPUT_FD);
		if (pids[i] == -1 || pids[i + process_count] == -1) {
			puts_with_color("test_sync: ERROR creating process\n", 0xFF0000);
			sys_sem_close_named(SEM_NAME);
			return -1;
		}
	}

	int64_t status1;
	int64_t status2;

	for (i = 0; i < process_count; i++) {
		sys_wait_pid(pids[i], &status1);
		sys_wait_pid(pids[i + process_count], &status2);
	}

	sys_sem_close_named(SEM_NAME);

	printf("Final value: %d\n", global);

	return 0;
}
