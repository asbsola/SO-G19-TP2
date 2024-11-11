#ifndef __IDLE__H__
#define __IDLE__H__

#include <drivers/videoDriver.h>
#include <interruptHandlers/interrupts.h>
#include <process.h>
#include <screen_service.h>
#include <def.h>

uint64_t idle(char **argv, int argc);

#endif