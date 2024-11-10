#ifndef __PROCESS__H__
#define __PROCESS__H__

#include <stdint.h>

void exit_self(int64_t status);
void kill_self();
void block_self();
void unblock_self();

#endif
