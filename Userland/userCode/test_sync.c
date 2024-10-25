#include <stdint.h>
#include <syscall_adapters.h>
#include <std.h>
#include <stddef.h>

#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  sys_yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t process_inc(char **argv, int argc) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 3)
    return -1;

  if ((n = atoi(argv[0])) <= 0)
    return -1;
  if ((inc = atoi(argv[1])) == 0)
    return -1;
  if ((use_sem = atoi(argv[2])) < 0)
    return -1;

  if (use_sem)
    if (sys_sem_open(SEM_ID, 1) == -1) {
      printf("test_sync: ERROR opening semaphore\n");
      return -1;
    }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      sys_sem_down(SEM_ID);
    slowInc(&global, inc);
    if (use_sem)
      sys_sem_up(SEM_ID);
  }

  if (use_sem)
    sys_sem_close(SEM_ID);

  return 0;
}

uint64_t test_sync(char **argv, uint64_t argc) { //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];
  

  if (argc != 3)
    return -1;

  printf("Starting test_sync\n");
  char *argvDec[] = {argv[1], "-1", argv[2], NULL};
  char *argvInc[] = {argv[1], "1", argv[2], NULL};

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

  printf("Final value: %d\n", global);

  return 0;
}