#include <stdint.h>
#include <drivers/videoDriver.h>
#include <interruptHandlers/interrupts.h>
#include <interruptHandlers/exceptions.h>
#include <shell_caller.h>

void exceptionDispatcher(int id, const registers64_t *registers) {
	write_to_video_text_buffer("\n", 1, HEX_WHITE);
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

	print_registers(registers, HEX_RED);

    update_frame_buffer();

	restart_shell();
}
