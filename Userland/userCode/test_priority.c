#include <stdint.h>
#include <stdio.h>
#include <def.h>
#include <syscall_adapters.h>
#include <test_utils.h>


#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 10000000      // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3

processPriority prio[TOTAL_PROCESSES] = {LOW, MEDIUM, HIGH};

void test_prio() {
  pid_t pids[TOTAL_PROCESSES];
  
  char *argvAux[] = {"endless_loop", NULL};
  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = sys_create_process(NOT_IN_FOREGROUND, endless_loop, argvAux);

  bussy_wait(WAIT);
  printf("\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_nicent(pids[i], prio[i]);

  bussy_wait(WAIT);
  printf("\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_block_process_by_pid(pids[i]);

  printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_nicent(pids[i], MEDIUM);

  printf("UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_unblock_process_by_pid(pids[i]);

  bussy_wait(WAIT);
  printf("\nKILLING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_kill_process_by_pid(pids[i]);
}