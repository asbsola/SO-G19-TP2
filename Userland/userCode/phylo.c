#include <syscall_adapters.h>
#include <test_utils.h>
#include <std.h>
#include <def.h>

#define Q_CODE_PRESSED 0x10
#define A_CODE_PRESSED 0x1E
#define R_CODE_PRESSED 0x13

#define MAX_NUMBER_OF_PHILOSOPHERS 25
#define INITIAL_NUMBER_OF_PHILOSOPHERS 5
#define VIEW_PIPE "view_philosophers_pipe"
#define ADD_REMOVE_MUTEX "add_remove_mutex"

typedef enum {EATING, THINKING} philosopherState;
typedef struct {
    philosopherState state;
    pid_t pid;
} philosopher;

philosopher philosophers[MAX_NUMBER_OF_PHILOSOPHERS];
fd_t view_pipe;
sem_t add_remove_mutex;
sem_t chopsticks[MAX_NUMBER_OF_PHILOSOPHERS];
uint32_t number_of_philosophers = INITIAL_NUMBER_OF_PHILOSOPHERS;

uint64_t view(char **argv, int argc) {
    char buffer[MAX_NUMBER_OF_PHILOSOPHERS * 2 + 2];
    while (sys_read(view_pipe, buffer, MAX_NUMBER_OF_PHILOSOPHERS * 2 + 1) != EOF) {
        buffer[MAX_NUMBER_OF_PHILOSOPHERS * 2 + 1] = '\0';
        puts(buffer);
    }

    return 0;
}

void write_philosophers_state_to_pipe() {
    char buffer[number_of_philosophers * 2 + 2];
    for (int i = 0; i < number_of_philosophers; i++) {
        if (philosophers[i].state == EATING) buffer[2 * i] = 'E';
        else buffer[2 * i] = '.';

        buffer[2 * i + 1] = ' ';
    }

    buffer[number_of_philosophers * 2] = '\n';

    sys_write(sys_get_stdout(), buffer, number_of_philosophers * 2 + 1);
}

void eat(uint64_t id) {
    philosophers[id].state = EATING;
    write_philosophers_state_to_pipe();

    sys_yield();
}

void think(uint64_t id) {
    philosophers[id].state = THINKING;
    write_philosophers_state_to_pipe();

    sys_yield();
}

uint64_t thinking_man(char **argv, int argc) {
    uint64_t id = atoi(argv[1]);

    while (1) {
        sys_sem_down(add_remove_mutex);
        uint64_t second_chopstick_index = (id + 1) % number_of_philosophers;
        sys_sem_up(add_remove_mutex);

        if (id % 2 == 0) {
            sys_sem_down(chopsticks[id]);
            sys_sem_down(chopsticks[second_chopstick_index]);
        } else {
            sys_sem_down(chopsticks[second_chopstick_index]);
            sys_sem_down(chopsticks[id]);
        }

        eat(id);

        sys_sem_up(chopsticks[id]);
        sys_sem_up(chopsticks[second_chopstick_index]);

        think(id);
    }

    return 0;
}

int add_philosopher() {
    sys_sem_down(add_remove_mutex);
    sys_sem_down(chopsticks[0]);

    number_of_philosophers++;

    philosophers[number_of_philosophers - 1].state = THINKING;

    char buff[20];
    itoa(number_of_philosophers - 1, buff, 20);
    char* argv[] = {"thinking_man", buff, 0};

    if ((philosophers[number_of_philosophers - 1].pid = sys_create_process(thinking_man, argv, KEYBOARD_INPUT_FD, view_pipe)) == -1) {
        sys_sem_up(chopsticks[0]);
        sys_sem_up(add_remove_mutex);
        return -1;
    }

    sys_sem_up(chopsticks[0]);
    sys_sem_up(add_remove_mutex);

    return 0;
}

int remove_philosopher() {
    sys_sem_down(add_remove_mutex);

    if ((number_of_philosophers - 1) % 2 == 0) {
        sys_sem_down(chopsticks[number_of_philosophers - 1]);
        sys_sem_down(chopsticks[0]);
    } else {
        sys_sem_down(chopsticks[0]);
        sys_sem_down(chopsticks[number_of_philosophers - 1]);
    }

    sys_kill_process_by_pid(philosophers[number_of_philosophers - 1].pid, 0);
    number_of_philosophers--;

    sys_sem_up(chopsticks[number_of_philosophers]);
    sys_sem_up(chopsticks[0]);
    sys_sem_up(add_remove_mutex);

    return 0;
}

uint64_t phylo(char **argv, int argc) {
    number_of_philosophers = 0;

    int in_background = argv[argc - 1][0] == '&';

    if (in_background) {
        puts_with_color("phylo: ERROR don't run me in background (no exit condition)\n", 0xFF0000);
        return -1;
    }

    puts_with_color("phylo instructions: 'a' to add - 'r' to remove - 'q' to quit\n\npress any key to start...\n", 0xc2daff);
    while (sys_get_key_pressed() == 0);

    if ((view_pipe = sys_pipe_open_named(VIEW_PIPE, NON_EOF_CONSUMER)) == -1) {
        puts_with_color("phylo: ERROR opening pipe\n", 0xFF0000);
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
            sys_sem_close(add_remove_mutex);
            return -1;
        }
    }

    for (int i = 0; i < INITIAL_NUMBER_OF_PHILOSOPHERS; i++) {
        if (add_philosopher() == -1) {
            puts_with_color("phylo: ERROR creating process\n", 0xFF0000);
            for (int j = 0; j < MAX_NUMBER_OF_PHILOSOPHERS; j++) {
                sys_sem_close(chopsticks[j]);
            }
            sys_sem_close(add_remove_mutex);
            return -1;
        }
    }

    char* args[] = {0};
    sys_create_process(view, args, KEYBOARD_INPUT_FD, SCREEN_OUTPUT_FD);

    char key;
    while ((key = sys_get_key_pressed()) != Q_CODE_PRESSED) {
        if (key == A_CODE_PRESSED && number_of_philosophers < MAX_NUMBER_OF_PHILOSOPHERS - 1)
            add_philosopher();
        else if (key == R_CODE_PRESSED && number_of_philosophers > 2)
            remove_philosopher();
    }

    while (sys_get_key_pressed() != 0);

    for (int i = 0; i < number_of_philosophers; i++) {
        sys_kill_process_by_pid(philosophers[i].pid, 0);
    }

    for (int j = 0; j < MAX_NUMBER_OF_PHILOSOPHERS; j++) {
        sys_sem_close(chopsticks[j]);
    }

    sys_pipe_close(view_pipe);

    sys_sem_close_named(ADD_REMOVE_MUTEX);
    return 0;
}
