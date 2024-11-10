#ifndef __EXCEPTIONS__H__
#define __EXCEPTIONS__H__

#include <registers.h>
#include <drivers/videoDriver.h>
#include <interruptHandlers/interrupts.h>
#include <stdint.h>


void exceptionDispatcher(int id, const registers64_t *registers);

#endif