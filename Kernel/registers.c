#include <registers.h>
#include <drivers/videoDriver.h>
#include <lib.h>

registers64_t registers_to_print;

void save_registers(const registers64_t *registers){
    registers_to_print = *registers;
}

void print_register(char * name, uint32_t nameDim, uint64_t value, uint32_t color) {
	char buffer[17];
	uint64_to_hex_string(value, buffer, 17);
	write_to_video_text_buffer("\t", 1, HEX_WHITE);
	write_to_video_text_buffer(name, nameDim, color);
	write_to_video_text_buffer(buffer, 16, color);
	write_to_video_text_buffer("\n", 1, HEX_WHITE);
    update_frame_buffer();
}

void print_stored_registers() {
    print_registers(&registers_to_print, HEX_WHITE);
}

void print_registers(const registers64_t *registers, uint32_t color) {
	print_register("cs:     ", 8, registers->cs, color);
	print_register("rflags: ", 8, registers->rflags, color);
	print_register("rsp:    ", 8, registers->ss_rsp, color);
	print_register("rbp:    ", 8, registers->rbp, color);
	print_register("rip:    ", 8, registers->rip, color);
	print_register("rax:    ", 8, registers->rax, color);
	print_register("rbx:    ", 8, registers->rbx, color);
	print_register("rcx:    ", 8, registers->rcx, color);
	print_register("rdx:    ", 8, registers->rdx, color);
	print_register("rsi:    ", 8, registers->rsi, color);
	print_register("rdi:    ", 8, registers->rdi, color);
	print_register("r8:     ", 8, registers->r8, color);
	print_register("r9:     ", 8, registers->r9, color);
	print_register("r10:    ", 8, registers->r10, color);
	print_register("r11:    ", 8, registers->r11, color);
	print_register("r12:    ", 8, registers->r12, color);
	print_register("r13:    ", 8, registers->r13, color);
	print_register("r14:    ", 8, registers->r14, color);
	print_register("r15:    ", 8, registers->r15, color);
}
