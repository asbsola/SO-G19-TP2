#include <stdint.h>
#include <drivers/videoDriver.h>
#include <lib.h>

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

void print_registers_by_params(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t rax, uint64_t rbx, uint64_t r10, uint64_t r11, uint64_t r12, uint64_t r13, uint64_t r14, uint64_t r15, uint64_t rip, uint64_t rsp, uint64_t rbp) {
	print_register("rsp:    ", 8, rsp);
	print_register("rbp:    ", 8, rbp);
	print_register("rip:    ", 8, rip);
	print_register("rax:    ", 8, rax);
	print_register("rbx:    ", 8, rbx);
	print_register("rcx:    ", 8, rcx);
	print_register("rdx:    ", 8, rdx);
	print_register("rsi:    ", 8, rsi);
	print_register("rdi:    ", 8, rdi);
	print_register("r8:     ", 8, r8);
	print_register("r9:     ", 8, r9);
	print_register("r10:    ", 8, r10);
	print_register("r11:    ", 8, r11);
	print_register("r12:    ", 8, r12);
	print_register("r13:    ", 8, r13);
	print_register("r14:    ", 8, r14);
	print_register("r15:    ", 8, r15);
}

void print_register(char * name, uint32_t nameDim, uint64_t value) {
	char buffer[17];
	uint64_to_hex_string(value, buffer, 17);
	print_tab();
	write_to_video_text_buffer(name, nameDim, HEX_RED);
	write_to_video_text_buffer(buffer, 16, HEX_RED);
	print_enter();
}

void print_tab() {
	write_to_video_text_buffer("    ", 4, HEX_WHITE);
}

void print_enter() {
	write_to_video_text_buffer("\n", 1, HEX_WHITE);
}

void uint64_to_hex_string(uint64_t num, char *buffer, uint16_t buffer_size) {
    // Asegúrate de que el buffer sea lo suficientemente grande para contener la cadena hexadecimal
    // y el carácter nulo de terminación. Para un entero de 64 bits necesitas 16 caracteres más el nulo.
    if (buffer_size < 17) {  // 16 caracteres para 64 bits en hex + 1 para el terminador nulo
        buffer[0] = '\0';
        return;
    }

    const char hex_digits[] = "0123456789ABCDEF";

    // Empieza desde el final del buffer
    buffer[16] = '\0';  // Terminador nulo
    for (int i = 15; i >= 0; --i) {
        buffer[i] = hex_digits[num & 0xF];  // Obtiene el último dígito hexadecimal
        num >>= 4;  // Desplaza el número 4 bits a la derecha
    }
}