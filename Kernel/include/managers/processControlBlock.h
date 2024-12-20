#ifndef __PCB__H__
#define __PCB__H__

#include <def.h>
#include <managers/semaphore.h>

typedef struct processControlBlockCDT {
	pid_t pid;
	pid_t parent_pid;
	processPriority priority;
	processStatus status;
	char *stack;
	char **argv;
	uint64_t rsp;
	uint64_t ret;
	uint8_t parent_is_waiting;
	semaphoreADT blocking_sem;
    sem_t pipe_mutex;
	fd_t stdin;
	fd_t stdout;
	int input_mode;
} processControlBlockCDT;

typedef struct processControlBlockCDT *processControlBlockADT;

#endif
