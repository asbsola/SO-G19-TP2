#include <stdint.h>
#include <drivers/videoDriver.h>
#include <interrupts.h>
#include <lib.h>
#include <exceptions.h>

void exceptionDispatcher(int id, const registers64_t *registers) {
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

	print_registers_by_params(registers);

    update_frame_buffer();

	haltcpu();
}
