#ifndef _VIDEO_H_
#define _VIDEO_H_

#include <stdint.h>

#define SCREEN_TEXT_BUFFER_HEIGHT 1000
#define SCREEN_TEXT_BUFFER_WIDTH 200

#define MIN_FONT_SIZE 4
#define CHAR_BIT_WIDTH 8
#define CHAR_BIT_HEIGHT 16
#define CHAR_BIT_X_SPACING 2
#define CHAR_BIT_Y_SPACING 2

#define HEX_WHITE 0x00FFFFFF
#define HEX_GRAY 0x00D8D8D8
#define HEX_RED 0x00FF0000

#define MAX_VIDEO_BUFFER_WIDTH 1200
#define MAX_VIDEO_BUFFER_HEIGHT 1200
#define MAX_VIDEO_BUFFER_BYTES_PER_PIXEL 3

extern unsigned char font_bitmap[4096];

void update_frame_buffer();
uint32_t get_video_buffer_width();
uint32_t get_video_buffer_height();

void put_pixel(uint32_t hexColor, uint64_t x, uint64_t y);
void clear_screen(uint32_t clearColor);
void draw_square(uint32_t hexColor, uint32_t posX, uint32_t posY, uint32_t size);
void draw_string(const char* str, uint32_t len, uint32_t hexColor, uint32_t posX, uint32_t posY);

void set_font_size(uint32_t fontSize);
void clear_video_text_buffer();
void write_to_video_text_buffer(const char* data, uint32_t data_len, uint32_t hexColor);
void update_screen_text_buffer();

#endif
