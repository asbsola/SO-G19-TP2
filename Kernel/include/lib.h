#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);

void print_registers_by_params(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t rax, uint64_t rbx, uint64_t rbp, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t r11, uint64_t r12, uint64_t r13, uint64_t r14, uint64_t r15, uint64_t rip, uint64_t rsp);

void print_register(char * name, uint32_t nameDim, uint64_t value);

void print_tab();

void print_enter();

void uint64_to_hex_string(uint64_t num, char *buffer, uint16_t buffer_size);

#endif