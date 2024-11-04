#include <managers/pipesManager.h>


typedef struct buffer{
    char buffer[BUFFER_SIZE];
    uint64_t reading_index;
    uint64_t writing_index;
    sem_t mutex;
    sem_t write_mutex;
    sem_t read_mutex;
    uint64_t blocked_readers;
    uint64_t blocked_writers;
    char * name;
} buffer;

typedef struct pipesManagerCDT {
    buffer * buffers[MAX_PIPES];
    fd_t last_fd;
    semaphoreManagerADT semaphore_manager;
    memoryManagerADT memory_manager;
} pipesManagerCDT;


pipesManagerADT init_pipes_manager(memoryManagerADT memory_manager, semaphoreManagerADT semaphore_manager) {
    pipesManagerADT pipes_manager = mem_alloc(memory_manager, sizeof(pipesManagerCDT));
    if (pipes_manager == NULL)
        return NULL;

    for (fd_t i = 0; i < MAX_PIPES; i++)
        pipes_manager->buffers[i] = NULL;

    pipes_manager->last_fd = 0;
    pipes_manager->semaphore_manager = semaphore_manager;
    pipes_manager->memory_manager = memory_manager;

    return pipes_manager;
}

fd_t get_lowest_unused_fd(pipesManagerADT pipes_manager) {
    for (fd_t i = 0; i < MAX_PIPES; i++) {
        if (pipes_manager->buffers[i] == NULL)
            return i;
    }
    return -1;
}

fd_t open_pipe(pipesManagerADT pipes_manager) {
    fd_t fd = get_lowest_unused_fd(pipes_manager);
    if (fd == -1) return -1;

    pipes_manager->buffers[fd] = mem_alloc(pipes_manager->memory_manager, sizeof(buffer));
    if (pipes_manager->buffers[fd] == NULL) return -1;

    for(int i = 0; i < BUFFER_SIZE; i++)
        pipes_manager->buffers[fd]->buffer[i] = 0;

    pipes_manager->buffers[fd]->writing_index = 0;
    pipes_manager->buffers[fd]->reading_index = 0;
    pipes_manager->buffers[fd]->blocked_readers = 0;
    pipes_manager->buffers[fd]->blocked_writers = 0;

    pipes_manager->buffers[fd]->mutex = open_sem(pipes_manager->semaphore_manager, 1);
    pipes_manager->buffers[fd]->write_mutex = open_sem(pipes_manager->semaphore_manager, 0);
    pipes_manager->buffers[fd]->read_mutex = open_sem(pipes_manager->semaphore_manager, 0);

    return fd;
}

int close_pipe(pipesManagerADT pipes_manager, fd_t fd) {
    if (fd < 0 || fd >= MAX_PIPES || pipes_manager->buffers[fd] == NULL) return -1;

    close_sem(pipes_manager->semaphore_manager, pipes_manager->buffers[fd]->mutex);
    close_sem(pipes_manager->semaphore_manager, pipes_manager->buffers[fd]->write_mutex);
    close_sem(pipes_manager->semaphore_manager, pipes_manager->buffers[fd]->read_mutex);

    if(pipes_manager->buffers[fd]->name != NULL)
        mem_free(pipes_manager->memory_manager, pipes_manager->buffers[fd]->name);

    mem_free(pipes_manager->memory_manager, pipes_manager->buffers[fd]);
    pipes_manager->buffers[fd] = NULL;
    
    return 0;
}

int next_index(int index) {
    return (index + 1) % BUFFER_SIZE;
}

int write_pipe(pipesManagerADT pipes_manager, fd_t fd, char* buff, int size) {
    if (fd < 0 || fd >= MAX_PIPES || pipes_manager->buffers[fd] == NULL) return -1;
    buffer * buffer = pipes_manager->buffers[fd];

    

    int i = 0;
    while(i < size) {
        down_sem(pipes_manager->semaphore_manager, buffer->mutex);
        if(buffer->reading_index == next_index(buffer->writing_index)) {
            buffer->blocked_writers++;
            up_sem(pipes_manager->semaphore_manager, buffer->mutex);
            down_sem(pipes_manager->semaphore_manager, buffer->write_mutex);
        } else {
            buffer->buffer[buffer->writing_index] = buff[i];
            buffer->writing_index = next_index(buffer->writing_index);
            i++;
            
        }
    }

    while(buffer->blocked_readers > 0) {
        buffer->blocked_readers--;
        up_sem(pipes_manager->semaphore_manager, buffer->read_mutex);
    }

    up_sem(pipes_manager->semaphore_manager, buffer->mutex);
    return 0;   
}

int read_pipe(pipesManagerADT pipes_manager, fd_t fd, char* buff, int size) {
    if (fd < 0 || fd >= MAX_PIPES || pipes_manager->buffers[fd] == NULL) return -1;
    buffer * buffer = pipes_manager->buffers[fd];

    int i = 0;
    while(i < size) {
        down_sem(pipes_manager->semaphore_manager, buffer->mutex);
        if(buffer->writing_index == buffer->reading_index) {
            buffer->blocked_readers++;
            up_sem(pipes_manager->semaphore_manager, buffer->mutex);
            down_sem(pipes_manager->semaphore_manager, buffer->read_mutex);
        } else {
            buff[i] = buffer->buffer[buffer->reading_index];
            buffer->reading_index = next_index(buffer->reading_index);
            i++;
        }
    }

    while(buffer->blocked_writers > 0) {
        buffer->blocked_writers--;
        up_sem(pipes_manager->semaphore_manager, buffer->write_mutex);
    }

    up_sem(pipes_manager->semaphore_manager, buffer->mutex);
    return 0;
}

fd_t get_pipe_named(pipesManagerADT pipes_manager, char* name) {
    fd_t i;

    for(i = 0; i <= pipes_manager->last_fd; i++) {
        if(pipes_manager->buffers[i] != NULL && strcmp(pipes_manager->buffers[i]->name, name) == 0) {
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

    pipes_manager->buffers[i]->name = mem_alloc(pipes_manager->memory_manager, strlen(name) + 1);
    if(pipes_manager->buffers[i]->name == NULL) {
        close_pipe(pipes_manager, i);
        return -1;
    }
    str_cpy(pipes_manager->buffers[i]->name, name);

    return i;
}