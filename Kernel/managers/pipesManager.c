// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <managers/pipesManager.h>

typedef struct pipe {
	char buffer[BUFFER_SIZE];
	uint64_t reading_index;
	uint64_t writing_index;
	sem_t read_bytes_sem;
	sem_t write_bytes_sem;
	sem_t read_mutex;
	sem_t write_mutex;
	char *name;
	int mode;
	uint8_t eof;
} pipe;

typedef struct pipesManagerCDT {
	pipe *pipes[MAX_PIPES];
	fd_t last_fd;
	semaphoreManagerADT semaphore_manager;
	memoryManagerADT memory_manager;
    processManagerADT process_manager;
    schedulerADT scheduler;
} pipesManagerCDT;

pipesManagerADT init_pipes_manager(memoryManagerADT memory_manager, semaphoreManagerADT semaphore_manager, processManagerADT process_manager, schedulerADT scheduler) {
	pipesManagerADT pipes_manager = mem_alloc(memory_manager, sizeof(pipesManagerCDT));
	if (pipes_manager == NULL) return NULL;

	for (fd_t i = 0; i < MAX_PIPES; i++) pipes_manager->pipes[i] = NULL;

	pipes_manager->last_fd = 1;
	pipes_manager->semaphore_manager = semaphore_manager;
	pipes_manager->memory_manager = memory_manager;
    pipes_manager->process_manager = process_manager;
    pipes_manager->scheduler = scheduler;

	// stdout
	pipe_open(pipes_manager, EOF_CONSUMER);

	// stdin
	pipe_open(pipes_manager, EOF_CONSUMER);

	return pipes_manager;
}

fd_t get_lowest_unused_fd(pipesManagerADT pipes_manager) {
	for (fd_t i = 0; i < MAX_PIPES; i++) {
		if (pipes_manager->pipes[i] == NULL) return i;
	}
	return -1;
}

fd_t pipe_open(pipesManagerADT pipes_manager, int mode) {
	fd_t fd = get_lowest_unused_fd(pipes_manager);
	if (fd == -1) return -1;

	pipes_manager->pipes[fd] = mem_alloc(pipes_manager->memory_manager, sizeof(pipe));
	if (pipes_manager->pipes[fd] == NULL) return -1;

	pipes_manager->pipes[fd]->eof = 0;
	pipes_manager->pipes[fd]->writing_index = 0;
	pipes_manager->pipes[fd]->reading_index = 0;
	pipes_manager->pipes[fd]->mode = mode;
	pipes_manager->pipes[fd]->name = NULL;

	sem_t read_sem = sem_open(pipes_manager->semaphore_manager, 0);
	sem_t write_sem = sem_open(pipes_manager->semaphore_manager, BUFFER_SIZE);
	sem_t read_mutex = sem_open(pipes_manager->semaphore_manager, 1);
	sem_t write_mutex = sem_open(pipes_manager->semaphore_manager, 1);
	if (read_sem == -1 || write_sem == -1 || read_mutex == -1 || write_mutex == -1) {
		sem_close(pipes_manager->semaphore_manager, read_sem);
		sem_close(pipes_manager->semaphore_manager, write_sem);
		sem_close(pipes_manager->semaphore_manager, read_mutex);
		sem_close(pipes_manager->semaphore_manager, write_mutex);
		mem_free(pipes_manager->memory_manager, pipes_manager->pipes[fd]);
		pipes_manager->pipes[fd] = NULL;
		return -1;
	}
	pipes_manager->pipes[fd]->read_bytes_sem = read_sem;
	pipes_manager->pipes[fd]->write_bytes_sem = write_sem;
	pipes_manager->pipes[fd]->read_mutex = read_mutex;
	pipes_manager->pipes[fd]->write_mutex = write_mutex;

	pipes_manager->last_fd = MAX(pipes_manager->last_fd, fd);

	return fd;
}

int pipe_close(pipesManagerADT pipes_manager, fd_t fd) {
	if (fd < 0 || fd >= MAX_PIPES || pipes_manager->pipes[fd] == NULL) return -1;

	sem_close(pipes_manager->semaphore_manager, pipes_manager->pipes[fd]->read_bytes_sem);
	sem_close(pipes_manager->semaphore_manager, pipes_manager->pipes[fd]->write_bytes_sem);
	sem_close(pipes_manager->semaphore_manager, pipes_manager->pipes[fd]->read_mutex);
	sem_close(pipes_manager->semaphore_manager, pipes_manager->pipes[fd]->write_mutex);

	if (pipes_manager->pipes[fd]->name != NULL) mem_free(pipes_manager->memory_manager, pipes_manager->pipes[fd]->name);

	mem_free(pipes_manager->memory_manager, pipes_manager->pipes[fd]);
	pipes_manager->pipes[fd] = NULL;

	if (pipes_manager->last_fd == fd) {
		fd_t i;
		for (i = fd - 1; i >= 0 && pipes_manager->pipes[i] == NULL; i--);
		pipes_manager->last_fd = i;
	}

	return 0;
}

int next_index(int index) {
	return (index + 1) % BUFFER_SIZE;
}

int pipe_write(pipesManagerADT pipes_manager, fd_t fd, const char *buffer, int size) {
	if (fd < 0 || fd >= MAX_PIPES || pipes_manager->pipes[fd] == NULL) return -1;
	pipe *pipe = pipes_manager->pipes[fd];
	processControlBlockADT process = get_process(pipes_manager->process_manager, get_current_pid(pipes_manager->scheduler));
	int i = 0;
	sem_down(pipes_manager->semaphore_manager, pipe->write_mutex);
    process->pipe_mutex = pipe->write_mutex;

	while (i < size && !pipe->eof) {
		sem_down(pipes_manager->semaphore_manager, pipe->write_bytes_sem);

		if (pipes_manager->pipes[fd] == NULL) return -1;

		pipe->buffer[pipe->writing_index] = buffer[i++];
		pipe->writing_index = next_index(pipe->writing_index);
		sem_up(pipes_manager->semaphore_manager, pipe->read_bytes_sem);
	}

    process->pipe_mutex = -1;
	sem_up(pipes_manager->semaphore_manager, pipe->write_mutex);
	return i;
}

int pipe_read(pipesManagerADT pipes_manager, fd_t fd, char *buffer, int size) {
	if (fd < 0 || fd >= MAX_PIPES || pipes_manager->pipes[fd] == NULL) return EOF;
	pipe *pipe = pipes_manager->pipes[fd];
	processControlBlockADT process = get_process(pipes_manager->process_manager, get_current_pid(pipes_manager->scheduler));
	int i = 0;
	sem_down(pipes_manager->semaphore_manager, pipe->read_mutex);
    process->pipe_mutex = pipe->read_mutex;

	while (i < size) {
		sem_down(pipes_manager->semaphore_manager, pipe->read_bytes_sem);

		if (pipes_manager->pipes[fd] == NULL) return EOF;

		if (pipe->eof && pipe->reading_index == pipe->writing_index) {
			if (pipe->mode == EOF_CONSUMER && i == 0)
				pipe->eof = 0;
			else
				sem_up(pipes_manager->semaphore_manager, pipe->read_bytes_sem);

            process->pipe_mutex = -1;
			sem_up(pipes_manager->semaphore_manager, pipe->read_mutex);
			if (i == 0) return EOF;
			return i;
		}

		buffer[i++] = pipe->buffer[pipe->reading_index];
		pipe->reading_index = next_index(pipe->reading_index);
		sem_up(pipes_manager->semaphore_manager, pipe->write_bytes_sem);
	}

    process->pipe_mutex = -1;
	sem_up(pipes_manager->semaphore_manager, pipe->read_mutex);
	if (size != 0 && i == 0) return EOF;
	return i;
}

int send_eof(pipesManagerADT pipes_manager, fd_t fd) {
	if (fd < 0 || fd >= MAX_PIPES || pipes_manager->pipes[fd] == NULL) return -1;

	pipe *pipe = pipes_manager->pipes[fd];

	pipe->eof = 1;
	sem_up(pipes_manager->semaphore_manager, pipe->read_bytes_sem);

	return 0;
}

fd_t get_pipe_named(pipesManagerADT pipes_manager, char *name) {
	fd_t i;

	for (i = 0; i <= pipes_manager->last_fd; i++) {
		if (pipes_manager->pipes[i] != NULL && str_cmp(pipes_manager->pipes[i]->name, name) == 0) { return i; }
	}

	return -1;
}

fd_t pipe_open_named(pipesManagerADT pipes_manager, char *name, int mode) {
	fd_t i = get_pipe_named(pipes_manager, name);

	if (i != -1) return i;

	i = pipe_open(pipes_manager, mode);

	if (i == -1) return -1;

	pipes_manager->pipes[i]->name = mem_alloc(pipes_manager->memory_manager, str_len(name) + 1);
	if (pipes_manager->pipes[i]->name == NULL) {
		pipe_close(pipes_manager, i);
		return -1;
	}
	str_cpy(pipes_manager->pipes[i]->name, name);

	return i;
}
