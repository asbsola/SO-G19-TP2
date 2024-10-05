#ifndef __PROCESS_MANAGER_H__
#define __PROCESS_MANAGER_H__

#include <stdint.h>
#include <managers/memoryManager.h>

#define PROCESS_STACK_SIZE 4096
#define MAX_PROCESSES 15 
#define INIT_PROCESS_PID 0

typedef int64_t pid_t;
typedef enum { RUNNING, READY, BLOCKED, EXITED } processStatus;
typedef enum { LOW, MEDIUM, HIGH } processPriority;

struct processControlBlockCDT {
    pid_t pid;
    pid_t parent_pid;
    processPriority priority;
    processStatus status;
    char* stack;
    uint64_t rsp;
    uint64_t rip;
    uint64_t ret;
} processControlBlockCDT;
typedef struct processControlBlockCDT* processControlBlockADT;

typedef struct processManagerCDT* processManagerADT;

processManagerADT init_process_manager(memoryManagerADT memory_manager);

pid_t create_process(processManagerADT process_manager, pid_t parent_pid, uint64_t (*process_start)(char**, int), char* argv[]);

int exit_process(processManagerADT process_manager, pid_t pid, int64_t status);
int block_process(processManagerADT process_manager, pid_t pid);
int kill_process(processManagerADT process_manager, pid_t pid);
int unblock_process(processManagerADT process_manager, pid_t pid);

processControlBlockADT* get_pcbs(processManagerADT process_manager);

#endif
