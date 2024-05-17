#ifndef _VIDEO_H_
#define _VIDEO_H_

#include <stdint.h>

#define SCREEN_TEXT_BUFFER_SIZE 4096

#define MIN_FONT_SIZE 4
#define CHAR_BIT_WIDTH 8
#define CHAR_BIT_HEIGHT 16
#define CHAR_BIT_X_SPACING 2
#define CHAR_BIT_Y_SPACING 2

#define HEX_WHITE 0x00FFFFFF
#define HEX_RED 0x00FF0000

extern unsigned char font_bitmap[4096];

void put_pixel(uint32_t hexColor, uint64_t x, uint64_t y);
void clear_screen(uint32_t clearColor);
void draw_square(uint32_t hexColor, uint32_t posX, uint32_t posY, uint32_t size);

void set_font_size(uint32_t fontSize);
//uint64_t getFontSize();
void clear_video_text_buffer();
void write_to_video_text_buffer(const char* data, uint32_t data_len, uint32_t hexColor);
void update_screen_text_buffer();

#endif
