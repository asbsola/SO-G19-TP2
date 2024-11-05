#ifndef __UTILS_H
#define __UTILS_H

#include <stdint.h>
#define HEX_COLOR_LEN 6

uint32_t GetUint();
uint32_t GetUniform(uint32_t max);

uint32_t hex_color_string_to_number(const char* hex_color);
#endif
