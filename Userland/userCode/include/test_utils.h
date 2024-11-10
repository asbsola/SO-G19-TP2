#ifndef __TEST_UTILS__H__
#define __TEST_UTILS__H__

#include <stdint.h>

#define TOTAL_PROCESSES 250

uint32_t GetUint();
uint32_t GetUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);
int64_t satoi(char *str);
uint64_t bussy_wait_proc(char **argv, int argc);
uint64_t endless_loop(char **argv, int argc);
uint64_t endless_counter(char **argv, int argc);
uint64_t echo_test(char **argv, int argc);
void endless_loop_print(uint64_t wait);

#endif
