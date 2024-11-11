#ifndef __REGS__H__
#define __REGS__H__

#include <stdint.h>

typedef struct {
	uint64_t rax;
	uint64_t rbx;
	uint64_t rcx;
	uint64_t rdx;
	uint64_t rbp;
	uint64_t rdi;
	uint64_t rsi;
	uint64_t r8;
	uint64_t r9;
	uint64_t r10;
	uint64_t r11;
	uint64_t r12;
	uint64_t r13;
	uint64_t r14;
	uint64_t r15;
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} registers64_t;

void save_registers(const registers64_t *registers);

void print_stored_registers();

void print_registers(const registers64_t *registers, uint32_t color);

void print_register(char *name, uint32_t nameDim, uint64_t value, uint32_t color);

#endif
