// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <drivers/videoDriver.h>
#include <lib.h>
#include <registers.h>

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


