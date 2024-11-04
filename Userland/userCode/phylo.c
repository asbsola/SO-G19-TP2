#include <syscall_adapters.h>
#include <std.h>

#define MAX_NUMBER_OF_PHILOSOPHERS 25
#define NUMBER_OF_PHILOSOPHERS 5
#define VIEW_SEMAPHORE "view_philosophers"
#define PHILOSOPHERS_MUTEX "philosophers"

typedef enum {EATING, THINKING} philosopherState;
typedef struct {
    philosopherState state;
} philosopher;

philosopher philosophers[MAX_NUMBER_OF_PHILOSOPHERS];

uint64_t view(char **argv, int argc) {
    while (1) {
        sys_sem_down_named(VIEW_SEMAPHORE);
        for (int i = 0; i < NUMBER_OF_PHILOSOPHERS; i++) {
            if (philosophers[i].state == EATING) puts("E ");
            else puts(". ");
        }
        putchar('\n');
    }

    return 0;
}

void eat(uint64_t id) {
    sys_sem_down_named(PHILOSOPHERS_MUTEX);
    philosophers[id].state = EATING;
    sys_yield();
    sys_sem_up_named(PHILOSOPHERS_MUTEX);
}

void think(uint64_t id) {
    sys_sem_down_named(PHILOSOPHERS_MUTEX);
    philosophers[id].state = THINKING;
    sys_yield();
    sys_sem_up_named(PHILOSOPHERS_MUTEX);
}

uint64_t thinking_man(char **argv, int argc) {
    sys_sem_open_named(PHILOSOPHERS_MUTEX, 1);

    uint64_t id = atoi(argv[1]);

    while (1) {
        eat(id);
        think(id);
    }

    return 0;
}

uint64_t phylo(char **argv, int argc) {
    if (sys_sem_open_named(VIEW_SEMAPHORE, 0) == -1) {
        puts_with_color("phylo: ERROR opening semaphore\n", 0xFF0000);
        return -1;
    }

    sys_sem_close_named(VIEW_SEMAPHORE);
    return 0;
}
