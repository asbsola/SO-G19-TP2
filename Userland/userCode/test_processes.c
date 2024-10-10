#include <std.h>
#include <syscall_adapters.h>
#include <test_utils.h>
#include <def.h>
#include <stddef.h>

#define MAX_TEST_ITERS 100

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
    char *argvAux[] = {NULL};

    uint64_t iter = 0;

    /*
    if (argc != 1)
        return -1;

    if ((max_processes = satoi(argv[0])) <= 0)
        return -1;
        */
    max_processes = 4;

    p_rq p_rqs[max_processes];

    while (iter++ < MAX_TEST_ITERS) {

        puts_with_color("\n-------------------\n", 0xc2daff);
        puts_with_color("Creating processes...\n", 0xc2daff);
        // Create max_processes processes
        for (rq = 0; rq < max_processes; rq++) {
            p_rqs[rq].pid = sys_create_process(NOT_IN_FOREGROUND, endless_loop, argvAux);
            printf("pid: %ld\n", p_rqs[rq].pid);

            if (p_rqs[rq].pid == -1) {
                printf("test_processes: ERROR creating process\n");
                return -1;
            } else {
                p_rqs[rq].state = RUNNING_TEST;
                alive++;
            }
        }

        // Randomly kills, blocks or unblocks processes until every one has been killed
        while (alive > 0) {
            printf("alive: %d\n", alive);
            for (rq = 0; rq < max_processes; rq++) {
                action = GetUniform(100) % 2;

                switch (action) {
                    case 0:
                        if (p_rqs[rq].state == RUNNING_TEST || p_rqs[rq].state == BLOCKED_TEST) {
                            printf("KILLING pid: %ld\n", p_rqs[rq].pid);
                            if (sys_kill_process_by_pid(p_rqs[rq].pid) == -1) {
                                printf("test_processes: ERROR killing process\n");
                                return -1;
                            }
                            p_rqs[rq].state = KILLED_TEST;
                            alive--;
                        }
                        break;

                    case 1:
                        if (p_rqs[rq].state == RUNNING_TEST) {
                            printf("BLOCKING pid: %ld\n", p_rqs[rq].pid);
                            if (sys_block_process_by_pid(p_rqs[rq].pid) == -1) {
                                printf("test_processes: ERROR blocking process\n");
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
                    printf("UN BLOCKING pid: %ld\n", p_rqs[rq].pid);
                    if (sys_unblock_process_by_pid(p_rqs[rq].pid) == -1) {
                        printf("test_processes: ERROR unblocking process\n");
                        return -1;
                    }
                    p_rqs[rq].state = RUNNING_TEST;
                }
        }
    }

    return 0;
}
