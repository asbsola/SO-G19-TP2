#ifndef __PCB_H__
#define __PCB_H__

#include <def.h>
#include <managers/semaphore.h>

typedef struct processControlBlockCDT
{
    pid_t pid;
    pid_t parent_pid;
    processPriority priority;
    processStatus status;
    char *stack;
    char** argv;
    uint64_t rsp;
    uint64_t ret;
    uint8_t parent_is_waiting;
    semaphoreADT blocking_sem;
    fd_t stdin;
    fd_t stdout;
} processControlBlockCDT;

typedef struct processControlBlockCDT *processControlBlockADT;

#endif
