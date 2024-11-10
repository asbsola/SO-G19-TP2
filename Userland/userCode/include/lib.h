#ifndef __LIB__H__
#define __LIB__H__

#include <stdint.h>

void jump_to_dir(uint64_t dir);
void *memset(void *destination, int32_t c, uint64_t length);

#endif
