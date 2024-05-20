#ifndef __STD__
#define __STD__

#include <stdint.h>

uint32_t strlen(const char* s);
void putchar(char c);
void puts(const char *s);

#define ITOA_BUFF_MAX_SIZE 100
char* itoa(uint64_t num, char* dest, uint32_t dest_max_len);

#define PRINTF_PRINT_BUFF_MAX_SIZE 1024
void printf(const char* fmt, uint64_t arg);

#endif
