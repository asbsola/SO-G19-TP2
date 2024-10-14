// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <def.h>
#include <syscall_adapters.h>
#include <test_utils.h>
#include <stddef.h>
#include <std.h>

#define MILLIS 1000

processPriority prio[TOTAL_PROCESSES] = {LOW, MEDIUM, HIGH};

uint64_t test_prio(char **argv, int argc) {
    pid_t pids[TOTAL_PROCESSES];
    
    uint64_t max_processes;

    if (argc < 2 || (max_processes = satoi(argv[1])) <= 0) {
        puts_with_color("test_processes: ERROR must provide max_processes (tops at 250)\n", 0xFF0000);
        return -1;
    }
    max_processes = (max_processes > TOTAL_PROCESSES) ? TOTAL_PROCESSES : max_processes; 

    uint8_t in_background = (argc > 2 && argv[argc - 1][0] == '&');

    char *argvAux[] = {"endless_loop", NULL};
    uint64_t i;

    if (!in_background)
        puts_with_color("\nCREATING PROCESSES...\n", 0xc2daff);
    
    for (i = 0; i < max_processes; i++){
        pids[i] = sys_create_process(endless_loop, argvAux);
        if (pids[i] == -1) {
            puts_with_color("test_processes: ERROR creating process\n", 0xFF0000);
            return -1;
        }
    }

    bussy_wait(MILLIS);

    if (!in_background)
        puts_with_color("\nCHANGING PRIORITIES...\n", 0xc2daff);

    for (i = 0; i < max_processes; i++)
        if(sys_nicent(pids[i], prio[i]) == -1){
            puts_with_color("test_processes: ERROR changing priority\n", 0xFF0000);
            return -1;
        }

    bussy_wait(MILLIS);

    if (!in_background)
        puts_with_color("\nBLOCKING...\n", 0xc2daff);

    for (i = 0; i < max_processes; i++)
        if(sys_block_process_by_pid(pids[i]) == -1){
            puts_with_color("test_processes: ERROR blocking process\n", 0xFF0000);
            return -1;
        }

    bussy_wait(MILLIS);

    if (!in_background)
        puts_with_color("CHANGING PRIORITIES WHILE BLOCKED...\n", 0xc2daff);

    for (i = 0; i < max_processes; i++)
        if(sys_nicent(pids[i], MEDIUM)==-1){
            puts_with_color("test_processes: ERROR changing priority while blocked\n", 0xFF0000);
            return -1;
        }

    bussy_wait(MILLIS);
    
    if (!in_background)
        puts_with_color("UNBLOCKING...\n", 0xc2daff);

    for (i = 0; i < max_processes; i++)
        if(sys_unblock_process_by_pid(pids[i])==-1){
            puts_with_color("test_processes: ERROR unblocking process\n", 0xFF0000);
            return -1;
        }

    bussy_wait(MILLIS);

    if (!in_background)
        puts_with_color("\nKILLING...\n", 0xc2daff);

    for (i = 0; i < max_processes; i++)
        if(sys_kill_process_by_pid(pids[i])){
            puts_with_color("test_processes: ERROR killing process\n", 0xFF0000);
            return -1;
        }

    if (!in_background)
        puts_with_color("Test finished\n", 0x00FF00);

    return 0;
}
