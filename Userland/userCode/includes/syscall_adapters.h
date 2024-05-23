#ifndef __syscall_adapters__
#define __syscall_adapters__

#include <stdint.h>

uint64_t sys_read(const char* buff, uint64_t len);
uint64_t sys_write(uint64_t fd, const char* buff, uint64_t len);
void sys_put_text(const char* str, uint32_t len, uint32_t hexColor, uint32_t posX, uint32_t posY);
void sys_set_font_size(uint32_t font_size);
void sys_draw_square(uint32_t hexColor, uint32_t posX, uint32_t posY, uint32_t size);
void sys_get_time();
uint32_t sys_get_screen_width();
uint32_t sys_get_screen_height();
uint8_t sys_get_key_pressed();
char sys_get_character_pressed();
void sys_clear_text_buffer();
void sys_get_cpu_vendor(char* buff);
void sys_beep(uint64_t freq, uint64_t secs);
void sys_delay(uint64_t milis);
void sys_clear_screen(uint32_t clearColor);

#endif
