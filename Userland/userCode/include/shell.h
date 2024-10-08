#ifndef __SHELL__
#define __SHELL__

#include <syscall_adapters.h>
#include <std.h>
#include <cucaracha.h>
#include <eliminator.h>
#include <lib.h>


#define MAX_SHELL_INPUT 1024

void help();
void cls();
void info();
void font_size();
void time();
void regs();
void beep();
void song();
void calculator();
void eliminator();
void jump();
void test_mm();
void mem();
void ps();

void run_shell();

#endif
