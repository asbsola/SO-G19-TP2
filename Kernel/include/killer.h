#ifndef __KILLER__H__
#define __KILLER__H__

#include <managers/processManager.h>
#include <stdint.h>

int kill_signal(pid_t pid, uint64_t recursive);
uint64_t killer(char **argv, int argc);

#endif
