#ifndef __TEST_UTILS__
#define __TEST_UTILS__

#include <stdint.h>

uint32_t GetUint();
uint32_t GetUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);
int64_t satoi(char *str);
void bussy_wait(uint64_t n);
uint64_t endless_loop(char** argv, int argc);
uint64_t endless_counter(char** argv, int argc);
void endless_loop_print(uint64_t wait);
#endif
