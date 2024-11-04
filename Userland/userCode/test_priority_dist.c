// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <def.h>
#include <syscall_adapters.h>
#include <test_utils.h>
#include <stddef.h>
#include <std.h>

#define WAIT_MILLIS 200000

extern processPriority prio[TOTAL_PROCESSES];

extern int counters[];

uint64_t test_prio_dist(char **argv, int argc) {
    pid_t pids[TOTAL_PROCESSES];

    uint64_t max_processes;
    if (argc < 2 || (max_processes = satoi(argv[1])) <= 0) {
        puts_with_color("test_priority_dist: ERROR must provide max_processes (tops at 250)\n", 0xFF0000);
        return -1;
    }
    max_processes = (max_processes > TOTAL_PROCESSES) ? TOTAL_PROCESSES : max_processes; 

    uint8_t in_background = (argc >= 3 &&  argv[argc - 1][0] == '&');

    char *argvAux1[] = {"endless_counter", "0", NULL};
    char *argvAux2[] = {"endless_counter", "1", NULL};
    char *argvAux3[] = {"endless_counter", "2", NULL};
    char **argvs[3] = {argvAux1, argvAux2, argvAux3};

    if (!in_background)
        puts_with_color("Creating and blocking processes...\n", 0xc2daff);
    for (int i = 0; i < max_processes; i++){
        pids[i] = sys_create_process(endless_counter, argvs[i%3], KEYBOARD_INPUT_FD, SCREEN_OUTPUT_FD);
        if (pids[i] == -1) {
            puts_with_color("test_priority_dist: ERROR creating process\n", 0xFF0000);
            return -1;
        }
        if(sys_block_process_by_pid(pids[i]) == -1){
            puts_with_color("test_priority_dist: ERROR blocking process\n", 0xFF0000);
            return -1;
        }
        sys_nicent(pids[i], i%3);
    }

    if (!in_background)
        puts_with_color("Unblocking...\n", 0xc2daff);

    for(int i=0; i<max_processes; i++){
        if(sys_unblock_process_by_pid(pids[i]) == -1){
            puts_with_color("test_priority_dist: ERROR unblocking process\n", 0xFF0000);
            return -1;
        }
    }

    if (!in_background)
        puts_with_color("Waiting...\n", 0xc2daff);
    
    sleep(WAIT_MILLIS);
    
    for (int i = 0; i < max_processes; i++){
        if(sys_kill_process_by_pid(pids[i], 0) == -1){
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
