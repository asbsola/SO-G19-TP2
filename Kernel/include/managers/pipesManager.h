#ifndef __PIPES_MANAGER__H__
#define __PIPES_MANAGER__H__

#include <def.h>
#include <managers/memoryManager.h>
#include <managers/semaphoreManager.h>
#include <managers/processManager.h>
#include <managers/scheduler.h>
#include <utils/string.h>

typedef struct pipesManagerCDT *pipesManagerADT;

pipesManagerADT init_pipes_manager(memoryManagerADT memory_manager, semaphoreManagerADT semaphore_manager, processManagerADT process_manager, schedulerADT scheduler);

fd_t pipe_open(pipesManagerADT pipes_manager, int mode);
int pipe_close(pipesManagerADT pipes_manager, fd_t fd);
int pipe_write(pipesManagerADT pipes_manager, fd_t fd, const char *buffer, int size);
int pipe_read(pipesManagerADT pipes_manager, fd_t fd, char *buffer, int size);
int send_eof(pipesManagerADT pipes_manager, fd_t fd);

fd_t pipe_open_named(pipesManagerADT pipes_manager, char *name, int mode);

#endif
