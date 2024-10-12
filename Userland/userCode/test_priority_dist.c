#include <stdint.h>
#include <def.h>
#include <syscall_adapters.h>
#include <test_utils.h>
#include <stddef.h>
#include <std.h>

#define WAIT_MILLIS 10000
#define TOTAL_PROCESSES 99

extern processPriority prio[TOTAL_PROCESSES];

extern int counters[];

uint64_t test_prio_dist(char **argv, int argc) {
    pid_t pids[TOTAL_PROCESSES];

    uint8_t in_background = (argc > 1 && argv[argc - 1][0] == '&');

    char *argvAux1[] = {"endless_counter", "0", NULL};
    char *argvAux2[] = {"endless_counter", "1", NULL};
    char *argvAux3[] = {"endless_counter", "2", NULL};
    char **argvs[3] = {argvAux1, argvAux2, argvAux3};

    if (!in_background)
        puts_with_color("Creating processes...\n", 0xc2daff);
    for (int i = 0; i < TOTAL_PROCESSES; i++){
        pids[i] = sys_create_process(NOT_IN_FOREGROUND, endless_counter, argvs[i%3]);
        if (pids[i] == -1) {
            puts_with_color("test_priority_dist: ERROR creating process\n", 0xFF0000);
            return -1;
        }
        sys_nicent(pids[i], i%3);
    }

    if (!in_background)
        puts_with_color("Waiting...\n", 0xc2daff);
    sys_delay(WAIT_MILLIS);
    
    for (int i = 0; i < TOTAL_PROCESSES; i++){
        if(sys_kill_process_by_pid(pids[i]) == -1){
            puts_with_color("test_priority_dist: ERROR killing process\n", 0xFF0000);
            return -1;
        }
    }

    if (!in_background){
        puts_with_color("COUNTERS:\n", 0xc2daff);
        printf("LOW: %d, MEDIUM: %d, HIGH: %d\n", counters[0], counters[1], counters[2]);
        puts_with_color("Test finished\n", 0x00FF00);
    }

    return 0;
}
