#ifndef __syscall_adapters__
#define __syscall_adapters__

#include <int_types.h>

void sys_write(uint64_t fd, const char* buff, uint64_t len);
void sys_set_font_size(uint32_t font_size);

#endif
