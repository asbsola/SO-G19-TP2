#ifndef __IDLE__H__
#define __IDLE__H__

#include <stdint.h>

#include <drivers/videoDriver.h>
#include <interruptHandlers/interrupts.h>
#include <screen_service.h>
#include <shell_caller.h>
#include <process.h>


uint64_t idle(char **argv, int argc);

#endif