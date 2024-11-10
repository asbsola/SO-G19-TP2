#ifndef __STD__H__
#define __STD__H__

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <syscall_adapters.h>

uint32_t strlen(const char *s);
void strcat(char *dest, const char *str1, const char *str2);
uint32_t strcmp(const char *s1, const char *s2);
void strcpy(char *dest, const char *src);

void putchar(char c);
void puts(const char *s);

#define HEX_COLOR_STRING_LEN 6
void puts_with_color(const char *s, uint32_t hexColor);

#define ITOA_BUFF_MAX_SIZE 100
char *itoa(int64_t num, char *dest, uint32_t dest_max_len);

#define PRINTF_PRINT_BUFF_MAX_SIZE 2048
void printf(const char *fmt, ...);

char getchar();
int getline(char *buffer, int max_len);
uint32_t atoi(const char *s);

#define SCANF_BUFF_MAX_SIZE 1024
void scanf(const char *fmt, void *ptr);

int isAlpha(char c);

int isVowel(char c);

void sleep(uint64_t milis);

char **split(const char *input, int *len, char delimiter);
void trim(char *str);

#endif
