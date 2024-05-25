#ifndef __exceptions__h__
#define __exceptions__h__

#include <registers.h>

void exceptionDispatcher(int id, const registers64_t *registers);

#endif