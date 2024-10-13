#include <std.h>
#include <syscall_adapters.h>
#include <test_utils.h>
#include <def.h>
#include <stddef.h>

#define MAX_TEST_ITERS 300
#define TOTAL_PROCESSES 250

enum State { RUNNING_TEST,
    BLOCKED_TEST,
    KILLED_TEST };

typedef struct P_rq {
    pid_t pid;
    enum State state;
} p_rq;

uint64_t test_processes(char **argv, int argc) {
    uint8_t rq;
    uint8_t alive = 0;
    uint8_t action;
    uint64_t max_processes;
    char *argvAux[] = {"endless_loop", NULL};
    p_rq p_rqs[TOTAL_PROCESSES];

    uint64_t iter = 0;

    if (argc < 2 || (max_processes = satoi(argv[1])) <= 0) {
        puts_with_color("test_processes: ERROR must provide max_processes (tops at 250)\n", 0xFF0000);
        return -1;
    }

    uint8_t in_background = (argc > 2 && argv[argc - 1][0] == '&');

    max_processes = (max_processes > TOTAL_PROCESSES) ? TOTAL_PROCESSES : max_processes; 

    

    while (iter++ < MAX_TEST_ITERS) {
        if (!in_background) {
            puts_with_color("\n-------------------\n", 0xc2daff);
            puts_with_color("Testing processes -> ", 0xc2daff);
            printf("iter: %ld of %ld\n", iter, MAX_TEST_ITERS);
        }

        // Create max_processes processes
        for (rq = 0; rq < max_processes; rq++) {
            p_rqs[rq].pid = sys_create_process(endless_loop, argvAux);

            if (p_rqs[rq].pid == -1) {
                puts_with_color("test_processes: ERROR creating process\n", 0xFF0000);
                return -1;
            } else {
                p_rqs[rq].state = RUNNING_TEST;
                alive++;
            }
        }

        if (!in_background)
            printf("processes created: %ld\n", rq);

        // Randomly kills, blocks or unblocks processes until every one has been killed
        while (alive > 0) {
            for (rq = 0; rq < max_processes; rq++) {
                action = GetUniform(100) % 2;

                switch (action) {
                    case 0:
                        if (p_rqs[rq].state == RUNNING_TEST || p_rqs[rq].state == BLOCKED_TEST) {
                            if (sys_kill_process_by_pid(p_rqs[rq].pid) == -1) {
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
