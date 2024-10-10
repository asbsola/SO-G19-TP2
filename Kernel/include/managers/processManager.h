#ifndef __PROCESS_MANAGER_H__
#define __PROCESS_MANAGER_H__

#include <stdint.h>
#include <managers/memoryManager.h>
#include <managers/scheduler.h>
#include <managers/processControlBlock.h>
#include <interruptHandlers/interrupts.h>
#include <def.h>

#define PROCESS_STACK_SIZE 4096
#define MAX_PROCESSES 15 
#define IDLE_PROCESS_PID 0

typedef struct processManagerCDT *processManagerADT;

processManagerADT init_process_manager(memoryManagerADT memory_manager, schedulerADT scheduler);

pid_t create_process(processManagerADT process_manager, pid_t parent_pid, uint8_t is_in_foreground, uint64_t (*process_start)(char **, int), char *argv[]);

int exit_process(processManagerADT process_manager, pid_t pid, int64_t status);
int block_process(processManagerADT process_manager, pid_t pid);
int kill_process(processManagerADT process_manager, pid_t pid);
int unblock_process(processManagerADT process_manager, pid_t pid);
int remove_process(processManagerADT process_manager, pid_t pid);

processControlBlockADT* get_processes(processManagerADT process_manager);
uint64_t get_num_processes(processManagerADT process_manager);
uint64_t get_max_pid(processManagerADT process_manager);
uint64_t get_ps_data(processManagerADT process_manager, memoryManagerADT mem_manager);
int wait_process(processManagerADT process_manager, pid_t pid);

#endif
