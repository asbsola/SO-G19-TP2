#ifndef __STRINGS_H__
#define __STRINGS_H__

#include <stdint.h>
uint64_t str_len(const char* str);
void str_cpy(char* dest, const char* src);
int str_cmp(const char* str1, const char* str2);

#endif
