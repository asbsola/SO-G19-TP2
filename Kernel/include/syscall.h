#ifndef _SYSCALL_H_
#define _SYSCALL_H_
#include <stdint.h>

uint64_t syscall_handler(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9);

#endif