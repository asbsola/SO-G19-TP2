#include <stdint.h>
#include <drivers/videoDriver.h>
#include <interrupts.h>
#include <lib.h>

void exceptionDispatcher(int id, uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx, uint64_t rsi, uint64_t rdi, uint64_t rbp, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t r11, uint64_t r12, uint64_t r13, uint64_t r14, uint64_t r15, uint64_t rip, uint64_t ss_rsp) {
    clear_video_text_buffer();
	print_enter();
	write_to_video_text_buffer("Exception caught: ", 18, HEX_RED);
	switch (id)
	{
		case 0:
			// Divide by zero
			write_to_video_text_buffer("Divide by zero exception\n", 25, HEX_WHITE);
			break;
		case 6:
			// Invalid opcode
			write_to_video_text_buffer("Invalid opcode exception\n", 25, HEX_WHITE);
			break;
		default:
			break;
	}

	print_registers_by_params(rdi, rsi, rdx, rcx, r8, r9, rax, rbx, r10, r11, r12, r13, r14, r15, rip, ss_rsp, rbp);

	haltcpu();
}
