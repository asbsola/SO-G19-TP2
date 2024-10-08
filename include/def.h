#ifndef  __DEF_H

#define __DEF_H

#include <stdint.h>

typedef int64_t pid_t;
typedef enum
{
    RUNNING,
    READY,
    BLOCKED,
    EXITED
} processStatus;
typedef enum
{
    LOW,
    MEDIUM,
    HIGH
} processPriority;

typedef struct
{
    char name[256];
    pid_t pid;
    pid_t parent_pid;
    processPriority priority;
    void *stack_pointer;
    void *base_pointer;
    uint8_t is_foreground;
    processStatus status;
} process_info_t;

#endif