#include <syscall_adapters.h>
#include <std.h>
#include <def.h>

#define Q_CODE_PRESSED 0x10
#define A_CODE_PRESSED 0x1E
#define R_CODE_PRESSED 0x13

#define MAX_NUMBER_OF_PHILOSOPHERS 25
#define INITIAL_NUMBER_OF_PHILOSOPHERS 5
#define VIEW_SEMAPHORE "view_philosophers"
#define PHILOSOPHERS_MUTEX "philosophers"
#define ADD_REMOVE_MUTEX "add_remove_mutex"

typedef enum {EATING, THINKING} philosopherState;
typedef struct {
    philosopherState state;
    pid_t pid;
} philosopher;

philosopher philosophers[MAX_NUMBER_OF_PHILOSOPHERS];
sem_t view_sem;
sem_t philosopher_mutex;
sem_t add_remove_mutex;
sem_t chopsticks[MAX_NUMBER_OF_PHILOSOPHERS];
uint32_t number_of_philosophers = INITIAL_NUMBER_OF_PHILOSOPHERS;

uint64_t view(char **argv, int argc) {
    while (1) {
        sys_sem_down(view_sem);
        sys_sem_down(philosopher_mutex);
        for (int i = 0; i < number_of_philosophers; i++) {
            if (philosophers[i].state == EATING) puts("E ");
            else puts(". ");
        }
        putchar('\n');
        sys_sem_up(philosopher_mutex);
    }

    return 0;
}

void eat(uint64_t id) {
    sys_sem_down(philosopher_mutex);
    philosophers[id].state = EATING;
    sys_sem_up(philosopher_mutex);
    sys_sem_up(view_sem);
    sys_yield();
}

void think(uint64_t id) {
    sys_sem_down(philosopher_mutex);
    philosophers[id].state = THINKING;
    sys_sem_up(philosopher_mutex);
    sys_sem_up(view_sem);
    sys_yield();
}

uint64_t thinking_man(char **argv, int argc) {
    philosopher_mutex = sys_sem_open_named(PHILOSOPHERS_MUTEX, 1);

    uint64_t id = atoi(argv[1]);

    while (1) {
        if (id % 2 == 0) {
            sys_sem_down(chopsticks[id]);
            sys_sem_down(chopsticks[(id + 1) % number_of_philosophers]);
        } else {
            sys_sem_down(chopsticks[(id + 1) % number_of_philosophers]);
            sys_sem_down(chopsticks[id]);
        }

        eat(id);

        sys_sem_up(chopsticks[id]);
        sys_sem_up(chopsticks[(id + 1) % number_of_philosophers]);

        think(id);
    }

    return 0;
}

int add_philosopher() {
    sys_sem_down(add_remove_mutex);

    sys_sem_down(chopsticks[0]);

    philosophers[number_of_philosophers].state = THINKING;

    char buff[20];
    itoa(number_of_philosophers, buff, 20);
    char* argv[] = {"thinking_man", buff, 0};

    if ((philosophers[number_of_philosophers].pid = sys_create_process(thinking_man, argv, KEYBOARD_INPUT_FD, SCREEN_OUTPUT_FD)) == -1) {
        sys_sem_up(chopsticks[0]);
        sys_sem_up(add_remove_mutex);
        return -1;
    }

    number_of_philosophers++;

    sys_sem_up(chopsticks[0]);

    sys_sem_up(add_remove_mutex);

    return 0;
}

int remove_philosopher() {
    sys_sem_down(add_remove_mutex);

    sys_sem_down(chopsticks[0]);

    sys_kill_process_by_pid(philosophers[number_of_philosophers].pid, 0);
    number_of_philosophers--;

    // reset semaphore
    sys_sem_close(chopsticks[number_of_philosophers + 1]);
    if ((chopsticks[number_of_philosophers + 1] = sys_sem_open(1)) == -1) {
        sys_sem_up(chopsticks[0]);
        sys_sem_up(add_remove_mutex);
        return -1;
    }

    sys_sem_up(chopsticks[0]);
    sys_sem_up(add_remove_mutex);

    return 0;
}

uint64_t phylo(char **argv, int argc) {
    number_of_philosophers = 0;

    if ((view_sem = sys_sem_open_named(VIEW_SEMAPHORE, 0)) == -1) {
        puts_with_color("phylo: ERROR opening semaphore\n", 0xFF0000);
        return -1;
    }

    if ((add_remove_mutex = sys_sem_open_named(ADD_REMOVE_MUTEX, 1)) == -1) {
        puts_with_color("phylo: ERROR opening semaphore\n", 0xFF0000);
        return -1;
    }

    for (int i = 0; i < MAX_NUMBER_OF_PHILOSOPHERS; i++) {
        if ((chopsticks[i] = sys_sem_open(1)) == -1) {
            puts_with_color("phylo: ERROR opening semaphore\n", 0xFF0000);
            for (int j = 0; j < i; j++) {
                sys_sem_close(chopsticks[j]);
            }
            sys_sem_close_named(VIEW_SEMAPHORE);
            return -1;
        }
    }

    for (int i = 0; i < INITIAL_NUMBER_OF_PHILOSOPHERS; i++) {
        if (add_philosopher() == -1) {
            puts_with_color("phylo: ERROR creating process\n", 0xFF0000);
            for (int j = 0; j < MAX_NUMBER_OF_PHILOSOPHERS; j++) {
                sys_sem_close(chopsticks[j]);
            }
            sys_sem_close_named(VIEW_SEMAPHORE);
            return -1;
        }
    }

    char* args[] = {0};
    pid_t view_pid = sys_create_process(view, args, KEYBOARD_INPUT_FD, SCREEN_OUTPUT_FD);

    char key;
    while ((key = sys_get_key_pressed()) != Q_CODE_PRESSED) {
        if (key == A_CODE_PRESSED && number_of_philosophers < MAX_NUMBER_OF_PHILOSOPHERS - 1)
            add_philosopher();
        else if (key == R_CODE_PRESSED && number_of_philosophers > 1)
            remove_philosopher();
    }

    while (sys_get_key_pressed() != 0);

    for (int i = 0; i < number_of_philosophers; i++) {
        sys_kill_process_by_pid(philosophers[i].pid, 0);
    }
    
    sys_kill_process_by_pid(view_pid, 0);

    sys_sem_close_named(VIEW_SEMAPHORE);
    sys_sem_close_named(PHILOSOPHERS_MUTEX);
    return 0;
}
