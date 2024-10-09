#ifndef __PCB_H__
#define __PCB_H__

#include <def.h>
#define NOT_IN_FOREGROUND 0
#define IN_FOREGROUND 1

typedef struct processControlBlockCDT
{
    pid_t pid;
    pid_t parent_pid;
    processPriority priority;
    processStatus status;
    char *stack;
    uint64_t rsp;
    uint64_t ret;
    uint8_t is_in_foreground;
} processControlBlockCDT;

typedef struct processControlBlockCDT *processControlBlockADT;

#endif
