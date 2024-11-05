#ifndef __PIPES_MANAGER_H__
#define __PIPES_MANAGER_H__

#include <managers/processManager.h>
#include <managers/memoryManager.h>
#include <managers/semaphoreManager.h>
#include <utils/string.h>
#include <def.h>

typedef struct pipesManagerCDT *pipesManagerADT;

pipesManagerADT init_pipes_manager(memoryManagerADT memory_manager, semaphoreManagerADT semaphore_manager);

fd_t open_pipe(pipesManagerADT pipes_manager);
int close_pipe(pipesManagerADT pipes_manager, fd_t fd);
int write_pipe(pipesManagerADT pipes_manager, fd_t fd, const char * buffer, int size);
int read_pipe(pipesManagerADT pipes_manager, fd_t fd, char * buffer, int size);

fd_t open_pipe_named(pipesManagerADT pipes_manager, char* name);



#endif
