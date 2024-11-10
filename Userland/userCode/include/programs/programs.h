#ifndef __PROGRAMS__
#define __PROGRAMS__

#include <syscall_adapters.h>
#include <std.h>
#include <lib.h>
#include <stddef.h>
#include <def.h>
#include <lib.h>


uint64_t info(char** argv, int argc);
uint64_t time(char** argv, int argc);
uint64_t regs(char** argv, int argc);
uint64_t calculator(char** argv, int argc);
uint64_t jump(char** argv, int argc);
uint64_t mem(char** argv, int argc);
uint64_t ps(char** argv, int argc);
uint64_t loop(char** argv, int argc);
uint64_t cat(char** argv, int argc);
uint64_t wc(char** argv, int argc);
uint64_t filter(char** argv, int argc);
uint64_t echo(char** argv, int argc);



#endif