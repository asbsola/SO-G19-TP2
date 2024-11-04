#include <managers/pipesManager.h>


typedef struct pipe{
    char buffer[BUFFER_SIZE];
    uint64_t reading_index;
    uint64_t writing_index;
    sem_t mutex;
    sem_t write_sem;
    sem_t read_sem;
    uint64_t blocked_readers;
    uint64_t blocked_writers;
    char * name;
} pipe;

typedef struct pipesManagerCDT {
    pipe * pipes[MAX_PIPES];
    fd_t last_fd;
    semaphoreManagerADT semaphore_manager;
    memoryManagerADT memory_manager;
} pipesManagerCDT;


pipesManagerADT init_pipes_manager(memoryManagerADT memory_manager, semaphoreManagerADT semaphore_manager) {
    pipesManagerADT pipes_manager = mem_alloc(memory_manager, sizeof(pipesManagerCDT));
    if (pipes_manager == NULL)
        return NULL;

    for (fd_t i = 0; i < MAX_PIPES; i++)
        pipes_manager->pipes[i] = NULL;

    pipes_manager->last_fd = 0;
    pipes_manager->semaphore_manager = semaphore_manager;
    pipes_manager->memory_manager = memory_manager;

    return pipes_manager;
}

fd_t get_lowest_unused_fd(pipesManagerADT pipes_manager) {
    for (fd_t i = 0; i < MAX_PIPES; i++) {
        if (pipes_manager->pipes[i] == NULL)
            return i;
    }
    return -1;
}

fd_t open_pipe(pipesManagerADT pipes_manager) {
    fd_t fd = get_lowest_unused_fd(pipes_manager);
    if (fd == -1) return -1;

    pipes_manager->pipes[fd] = mem_alloc(pipes_manager->memory_manager, sizeof(pipe));
    if (pipes_manager->pipes[fd] == NULL) return -1;

    for(int i = 0; i < BUFFER_SIZE; i++)
        pipes_manager->pipes[fd]->buffer[i] = 0;

    pipes_manager->pipes[fd]->writing_index = 0;
    pipes_manager->pipes[fd]->reading_index = 0;
    pipes_manager->pipes[fd]->blocked_readers = 0;
    pipes_manager->pipes[fd]->blocked_writers = 0;

    pipes_manager->pipes[fd]->mutex = open_sem(pipes_manager->semaphore_manager, 1);
    pipes_manager->pipes[fd]->write_sem = open_sem(pipes_manager->semaphore_manager, 0);
    pipes_manager->pipes[fd]->read_sem = open_sem(pipes_manager->semaphore_manager, 0);

    return fd;
}

int close_pipe(pipesManagerADT pipes_manager, fd_t fd) {
    if (fd < 0 || fd >= MAX_PIPES || pipes_manager->pipes[fd] == NULL) return -1;

    close_sem(pipes_manager->semaphore_manager, pipes_manager->pipes[fd]->mutex);
    close_sem(pipes_manager->semaphore_manager, pipes_manager->pipes[fd]->write_sem);
    close_sem(pipes_manager->semaphore_manager, pipes_manager->pipes[fd]->read_sem);

    if(pipes_manager->pipes[fd]->name != NULL)
        mem_free(pipes_manager->memory_manager, pipes_manager->pipes[fd]->name);

    mem_free(pipes_manager->memory_manager, pipes_manager->pipes[fd]);
    pipes_manager->pipes[fd] = NULL;
    
    return 0;
}

int next_index(int index) {
    return (index + 1) % BUFFER_SIZE;
}

int write_pipe(pipesManagerADT pipes_manager, fd_t fd, char* buffer, int size) {
    if (fd < 0 || fd >= MAX_PIPES || pipes_manager->pipes[fd] == NULL) return -1;
    pipe * pipe = pipes_manager->pipes[fd];

    

    int i = 0;
    while(i < size) {
        down_sem(pipes_manager->semaphore_manager, pipe->mutex);
        if(pipe->reading_index == next_index(pipe->writing_index)) {
            pipe->blocked_writers++;
            up_sem(pipes_manager->semaphore_manager, pipe->mutex);
            down_sem(pipes_manager->semaphore_manager, pipe->write_sem);
        } else {
            pipe->buffer[pipe->writing_index] = buffer[i];
            pipe->writing_index = next_index(pipe->writing_index);
            up_sem(pipes_manager->semaphore_manager, pipe->mutex);
            i++;
            
        }
    }

    down_sem(pipes_manager->semaphore_manager, pipe->mutex);
    while(pipe->blocked_readers > 0) {
        pipe->blocked_readers--;
        up_sem(pipes_manager->semaphore_manager, pipe->read_sem);
    }
    up_sem(pipes_manager->semaphore_manager, pipe->mutex);

    return 0;   
}

int read_pipe(pipesManagerADT pipes_manager, fd_t fd, char* buffer, int size) {
    if (fd < 0 || fd >= MAX_PIPES || pipes_manager->pipes[fd] == NULL) return -1;
    pipe * pipe = pipes_manager->pipes[fd];

    int i = 0;
    while(i < size) {
        down_sem(pipes_manager->semaphore_manager, pipe->mutex);
        if(pipe->writing_index == pipe->reading_index) {
            pipe->blocked_readers++;
            up_sem(pipes_manager->semaphore_manager, pipe->mutex);
            down_sem(pipes_manager->semaphore_manager, pipe->read_sem);
        } else {
            buffer[i] = pipe->buffer[pipe->reading_index];
            pipe->reading_index = next_index(pipe->reading_index);
            up_sem(pipes_manager->semaphore_manager, pipe->mutex);
            i++;
        }
    }
    down_sem(pipes_manager->semaphore_manager, pipe->mutex);
    while(pipe->blocked_writers > 0) {
        pipe->blocked_writers--;
        up_sem(pipes_manager->semaphore_manager, pipe->write_sem);
    }
    up_sem(pipes_manager->semaphore_manager, pipe->mutex);

    
    return 0;
}

fd_t get_pipe_named(pipesManagerADT pipes_manager, char* name) {
    fd_t i;

    for(i = 0; i <= pipes_manager->last_fd; i++) {
        if(pipes_manager->pipes[i] != NULL && str_cmp(pipes_manager->pipes[i]->name, name) == 0) {
            return i;
        }
    }

    return i;
}

fd_t open_pipe_named(pipesManagerADT pipes_manager, char* name){
    fd_t i = get_pipe_named(pipes_manager, name);
    
    if(i != -1)
        return i;
    
    i = open_pipe(pipes_manager);

    if(i == -1) return -1;

    pipes_manager->pipes[i]->name = mem_alloc(pipes_manager->memory_manager, str_len(name) + 1);
    if(pipes_manager->pipes[i]->name == NULL) {
        close_pipe(pipes_manager, i);
        return -1;
    }
    str_cpy(pipes_manager->pipes[i]->name, name);

    return i;
}

int close_pipe_named(pipesManagerADT pipes_manager, char* name) {
    fd_t i = get_pipe_named(pipes_manager, name);

    if(i == -1) return -1;

    return close_pipe(pipes_manager, i);
}