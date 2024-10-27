// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <syscall_adapters.h>
#include <test_utils.h>
#include <std.h>
#include <stddef.h>

#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 10

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  sys_yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t process_inc(char **argv, int argc) {
  uint64_t n;
  int64_t inc;
  int64_t use_sem;

  if (argc != 4)
    return -1;

  if ((n = satoi(argv[1])) <= 0) {
    puts_with_color("test_sync: ERROR error max_iters must be greater than 0\n", 0xFF0000);
    return -1;
  }
  if ((inc = satoi(argv[2])) == 0)
    return -1;
  if ((use_sem = satoi(argv[3])) < 0) {
    puts_with_color("test_sync: ERROR error max_iters must be greater than 0\n", 0xFF0000);
    return -1;
  }

  if (use_sem) {
    if (sys_sem_open(SEM_ID, 1) == -1) {
      puts_with_color("test_sync: ERROR opening semaphore\n", 0xFF0000);
      return -1;
    }
    printf("test_sync: Semaphore opened\n");
  }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      sys_sem_down(SEM_ID);
    slowInc(&global, inc);
    if (use_sem)
      sys_sem_up(SEM_ID);
  }

  return 0;
}

uint64_t test_sync(char **argv, uint64_t argc) {
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 3) {
    puts_with_color("test_processes: ERROR must provide max_iters and use_syncro (0 is no syncro - 1 is syncro).\n", 0xFF0000);
    return -1;
  }

  printf("Starting test_sync\n");
  char *argvDec[] = {"process_inc", argv[1], "-1", argv[2], NULL};
  char *argvInc[] = {"process_inc", argv[1], "1", argv[2], NULL};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = sys_create_process(process_inc, argvDec);
    pids[i + TOTAL_PAIR_PROCESSES] = sys_create_process(process_inc, argvInc);
  }

  int64_t status1;
  int64_t status2;

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    sys_wait_pid(pids[i] , &status1);
    sys_wait_pid(pids[i + TOTAL_PAIR_PROCESSES], &status2);
  }

  sys_sem_close(SEM_ID);

  printf("Final value: %d\n", global);

  return 0;
}
