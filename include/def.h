#ifndef __DEF_H
#define __DEF_H

#include <stdint.h>

//------------------------------------------------------------

#define PROCESS_STACK_SIZE (1 << 13)  // 8KB
#define MAX_PROCESSES 256
#define MAX_SEMAPHORES 256
#define MEMORY_MANAGER_MEM_SIZE (1 << 22)  // 4MB
#define MAX_PIPES 256
#define BUFFER_SIZE 256
//------------------------------------------------------------
#define PIT_FREQUENCY 60

#define NOT_IN_FOREGROUND 0
#define IN_FOREGROUND 1
#define PRIORITY_LEVELS 3

#define NOT_WAITING 0
#define WAITING 1

#define KEYBOARD_INPUT_FD 0
#define SCREEN_OUTPUT_FD 1

#define NON_EOF_CONSUMER 0
#define EOF_CONSUMER 1

#define EOF -1

#define NON_CANNONICAL 0
#define CANNONICAL 1

typedef int64_t pid_t;
typedef int64_t sem_t;
typedef int64_t fd_t;

typedef enum { RUNNING, READY, BLOCKED, EXITED, KILLED } processStatus;
typedef enum { LOW, MEDIUM, HIGH } processPriority;

typedef struct {
	pid_t pid;
	pid_t parent_pid;
	processPriority priority;
	void *stack_pointer;
	void *base_pointer;
	char *name;
	uint8_t parent_is_waiting;
	processStatus status;
} process_info_t;

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#endif
