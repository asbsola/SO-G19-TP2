#include <registers.h>
#include <drivers/videoDriver.h>
#include <lib.h>

registers64_t registers_to_print;

void save_registers(const registers64_t *registers){
    registers_to_print = *registers;
}

void print_register(char * name, uint32_t nameDim, uint64_t value) {
	char buffer[17];
	uint64_to_hex_string(value, buffer, 17);
	print_tab();
	write_to_video_text_buffer(name, nameDim, HEX_RED);
	write_to_video_text_buffer(buffer, 16, HEX_RED);
	print_enter();
}

void print_stored_registers() {
    print_registers(&registers_to_print);
}

void print_registers(const registers64_t *registers) {
	print_register("rsp:    ", 8, registers->ss_rsp);
	print_register("rbp:    ", 8, registers->rbp);
	print_register("rip:    ", 8, registers->rip);
	print_register("rax:    ", 8, registers->rax);
	print_register("rbx:    ", 8, registers->rbx);
	print_register("rcx:    ", 8, registers->rcx);
	print_register("rdx:    ", 8, registers->rdx);
	print_register("rsi:    ", 8, registers->rsi);
	print_register("rdi:    ", 8, registers->rdi);
	print_register("r8:     ", 8, registers->r8);
	print_register("r9:     ", 8, registers->r9);
	print_register("r10:    ", 8, registers->r10);
	print_register("r11:    ", 8, registers->r11);
	print_register("r12:    ", 8, registers->r12);
	print_register("r13:    ", 8, registers->r13);
	print_register("r14:    ", 8, registers->r14);
	print_register("r15:    ", 8, registers->r15);
}