#include <stdint.h>

// exceptionDispatcher(exceptionCode, rax, rbx, rcx, rdx, rsi, rdi, rbp, r8, r9, r10, r11, r12, r13, r14, r15, rip, ss_rsp)
void exceptionDispatcher(int id, uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx, uint64_t rsi, uint64_t rdi, uint64_t rbp, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t r11, uint64_t r12, uint64_t r13, uint64_t r14, uint64_t r15, uint64_t rip, uint64_t ss_rsp) {
	switch (id)
	{
	case 0:
		// Divide by zero
		break;
	case 6:
		// Invalid opcode
		break;
	default:
		break;
	}
}