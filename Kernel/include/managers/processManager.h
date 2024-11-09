#ifndef __PROCESS_MANAGER_H__
#define __PROCESS_MANAGER_H__

#include <stdint.h>
#include <managers/memoryManager.h>
#include <managers/scheduler.h>
#include <managers/processControlBlock.h>
#include <interruptHandlers/interrupts.h>
#include <def.h>


#define IDLE_PROCESS_PID 0

typedef struct processManagerCDT *processManagerADT;

processManagerADT init_process_manager(memoryManagerADT memory_manager, schedulerADT scheduler);

pid_t create_process(processManagerADT process_manager, pid_t parent_pid, uint64_t (*process_start)(char**, int), char** argv, fd_t stdin, fd_t stdout);

int exit_process(processManagerADT process_manager, pid_t pid, int64_t status);
int block_process(processManagerADT process_manager, pid_t pid);
int kill_process(processManagerADT process_manager, pid_t pid, uint64_t recursive);
int unblock_process(processManagerADT process_manager, pid_t pid);
int remove_process(processManagerADT process_manager, pid_t pid);

processControlBlockADT* get_processes(processManagerADT process_manager);
processControlBlockADT get_process(processManagerADT process_manager, pid_t pid);

pid_t get_parent_pid(processManagerADT process_manager, pid_t pid);
uint64_t get_num_processes(processManagerADT process_manager);
uint64_t get_max_pid(processManagerADT process_manager);
uint64_t get_ps_data(processManagerADT process_manager, memoryManagerADT mem_manager);
uint64_t wait(processManagerADT process_manager, int64_t* ret);
uint64_t wait_process(processManagerADT process_manager, pid_t child_pid, int64_t *ret);
uint64_t nicent(processManagerADT process_manager, pid_t pid, processPriority priority);

uint64_t kill_signal(processManagerADT process_manager, int recursive);

fd_t get_stdin(processManagerADT process_manager, pid_t pid);
fd_t get_stdout(processManagerADT process_manager, pid_t pid);

#endif
