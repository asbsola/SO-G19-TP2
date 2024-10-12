#ifndef __lib__h__
#define __lib__h__

#include <stdint.h>

void jump_to_dir(uint64_t dir);
void _hlt();
void * memset(void * destination, int32_t c, uint64_t length);

#endif
