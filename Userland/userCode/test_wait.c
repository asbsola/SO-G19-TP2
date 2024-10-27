// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <std.h>
#include <syscall_adapters.h>
#include <test_utils.h>
#include <def.h>
#include <stddef.h>

#define TOTAL_PROCESSES 250
#define MILLIS 300

enum State { RUNNING_TEST,
    ENDED_TEST};

typedef struct P_rq {
    pid_t pid;
    enum State state;
} p_rq;

uint64_t test_wait(char **argv, int argc) {
    uint64_t rq;
    uint8_t working = 0;
    uint8_t action;
    uint64_t max_processes;
    uint64_t max_iters;
    p_rq p_rqs[TOTAL_PROCESSES];

    uint64_t iter = 0;

     if (argc < 3) {
        puts_with_color("test_wait: ERROR must provide max_iters and max_processes (tops at 250)\n", 0xFF0000);
        return -1;
    }

    max_iters = satoi(argv[1]);
    if (max_iters <= 0) {
        puts_with_color("test_wait: ERROR max_iters must be greater than 0\n", 0xFF0000);
        return -1;
    }

    max_processes = satoi(argv[2]);
    if (max_processes <= 0) {
        puts_with_color("test_wait: ERROR max_processes must be greater than 0\n", 0xFF0000);
        return -1;
    }
    if(max_processes > TOTAL_PROCESSES){
        puts_with_color("test_wait: ERROR max_processes must be less than 250\n", 0xFF0000);
        return -1;
    }

    uint8_t in_background = (argc > 3 && argv[argc - 1][0] == '&');

    max_processes = (max_processes > TOTAL_PROCESSES) ? TOTAL_PROCESSES : max_processes; 

    while (iter++ < max_iters) {
        int64_t sum = 0;
        int64_t expected_sum = 0;
        if (!in_background) {
            puts_with_color("\n-------------------\n", 0xc2daff);
            puts_with_color("Testing processes -> ", 0xc2daff);
            printf("iter: %ld of %ld\n", iter, max_iters);
        }

        // Create max_processes processes with random delays
        for (rq = 0; rq < max_processes; rq++) {
            action = GetUniform(100) % 2;
            char rq_str[10];
            char millis_param[10];
            itoa(rq, rq_str, 10);
            itoa(MILLIS*action*2, millis_param, 10);
            char *argvAux[] = {"echo", rq_str, millis_param, NULL};
            p_rqs[rq].pid = sys_create_process(echo, argvAux);

            if (p_rqs[rq].pid == -1) {
                puts_with_color("test_wait: ERROR creating process\n", 0xFF0000);
                return -1;
            } else {
                p_rqs[rq].state = RUNNING_TEST;
                working++;
                expected_sum += rq;
            }
        }

        if (!in_background)
            printf("processes created: %ld\n", rq);

        if (!in_background)
            printf("give them time to finnish...\n");

        bussy_wait(MILLIS);

        if (!in_background)
            printf("waiting processes...\n");

        // Randomly waits with one syscall, until all have been retrieved
        while (working > 0) {
            for (rq = 0; rq < max_processes; rq++) {
                action = GetUniform(100) % 2;

                if (p_rqs[rq].state == RUNNING_TEST && action == 0) {
                    int64_t ret;
                    if (sys_wait_pid(p_rqs[rq].pid, &ret) == -1) {
                        puts_with_color("test_wait: ERROR waiting process\n", 0xFF0000);
                        return -1;
                    }
                    p_rqs[rq].state = ENDED_TEST;
                    sum += ret;
                    working--;
                }
            }
        }

        // Create max_processes processes with random delays
        for (rq = 0; rq < max_processes; rq++) {
            action = GetUniform(100) % 2;
            char rq_str[10];
            char millis_param[10];
            itoa(rq, rq_str, 10);
            itoa(MILLIS*2, millis_param, 10);
            char *argvAux[] = {"echo", rq_str, millis_param, NULL};
            p_rqs[rq].pid = sys_create_process(echo, argvAux);

            if (p_rqs[rq].pid == -1) {
                puts_with_color("test_wait: ERROR creating process\n", 0xFF0000);
                return -1;
            } else {
                p_rqs[rq].state = RUNNING_TEST;
                working++;
                expected_sum += rq;
            }
        }

        if (!in_background)
            printf("processes created: %ld\n", rq);

        if (!in_background)
            printf("give them time to finnish...\n");

        bussy_wait(MILLIS);

        if (!in_background)
            printf("waiting processes...\n");

        // Waits with the other syscalls, until all have been retrieved
        while (working > 0) {
            int64_t ret;
            if (sys_wait(&ret) == -1) {
                puts_with_color("test_wait: ERROR waiting process\n", 0xFF0000);
                return -1;
            }
            sum += ret;
            working--;
        }

        if(sum != expected_sum){
            puts_with_color("test_wait: ERROR values don't match\n", 0xFF0000);
            return -1;
        } else if(!in_background){
            printf("expected_sum: %d, sum: %d\n", expected_sum, sum);
            puts_with_color("test_wait: all process waited correctly\n", 0x00FF00);
        }
    }
    return 0;
}
