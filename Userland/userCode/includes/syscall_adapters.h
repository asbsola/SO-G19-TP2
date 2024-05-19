#ifndef __syscall_adapters__
#define __syscall_adapters__

#include <int_types.h>

void sys_write(uint64_t fd, const char* buff, uint64_t len);
void sys_put_text(const char* str, uint32_t len, uint32_t hexColor, uint32_t posX, uint32_t posY);
void sys_set_font_size(uint32_t font_size);
void sys_draw_square(uint32_t hexColor, uint32_t posX, uint32_t posY, uint32_t size);
uint32_t sys_get_screen_width();
uint32_t sys_get_screen_height();

#endif
