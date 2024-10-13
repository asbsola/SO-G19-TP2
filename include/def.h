#ifndef  __DEF_H
#define __DEF_H

#include <stdint.h>

#define NOT_IN_FOREGROUND 0
#define IN_FOREGROUND 1
#define PRIORITY_LEVELS 3

#define NOT_WAITING 0
#define WAITING 1

typedef int64_t pid_t;
typedef enum
{
    RUNNING,
    READY,
    BLOCKED,
    EXITED,
    KILLED
} processStatus;
typedef enum
{
    LOW,
    MEDIUM,
    HIGH
} processPriority;

typedef struct
{
    pid_t pid;
    pid_t parent_pid;
    processPriority priority;
    void *stack_pointer;
    void *base_pointer;
    const char* name;
    uint8_t parent_is_waiting;
    processStatus status;
} process_info_t;

#endif
