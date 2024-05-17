#ifndef _VIDEO_H_
#define _VIDEO_H_

#include <stdint.h>

#define SCREEN_TEXT_BUFFER_SIZE 4096

#define MIN_FONT_SIZE 4
#define CHAR_BIT_WIDTH 8
#define CHAR_BIT_HEIGHT 16
#define CHAR_BIT_X_SPACING 2
#define CHAR_BIT_Y_SPACING 2

extern unsigned char font_bitmap[4096];

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);
void clearScreen(uint32_t clearColor);
void drawSquare(uint32_t hexColor, uint32_t posX, uint32_t posY, uint32_t size);

void setFontSize(uint32_t fontSize);
//uint64_t getFontSize();
void clearVideoTextBuffer();
void writeToVideoTextBuffer(const char* data, uint32_t data_len, uint32_t hexColor);
void updateScreenTextBuffer();

#endif
