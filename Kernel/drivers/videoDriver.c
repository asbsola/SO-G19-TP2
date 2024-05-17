#include <stdint.h>
#include <drivers/videoDriver.h>

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    if (y > VBE_mode_info->height || x > VBE_mode_info->width)
        return;

    uint8_t * framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}

void drawSquare(uint32_t hexColor, uint32_t posX, uint32_t posY, uint32_t size) {
    for (uint32_t y = posY; y < posY + size; y++)
        for (uint32_t x = posX; x < posX + size; x++)
            putPixel(hexColor, x, y);
}

void clearScreen(uint32_t clearColor) {
    for (uint32_t y = 0; y < VBE_mode_info->height; y++)
        for (uint32_t x = 0; x < VBE_mode_info->width; x++)
            putPixel(clearColor, x, y);
}

typedef struct {
    uint32_t hexColor;
    char c;
} ScreenTextChar;

struct ScreenTextInfo {
    uint32_t index;
    uint32_t fontSize;
    ScreenTextChar buffer[SCREEN_TEXT_BUFFER_SIZE];
} screenTextInfo;

void setFontSize(uint32_t fontSize) {
    screenTextInfo.fontSize = fontSize;
}

void clearVideoTextBuffer() {
    clearScreen(0);

    for (uint32_t i = 0; i < SCREEN_TEXT_BUFFER_SIZE; i++) {
        screenTextInfo.buffer[i].c = ' ';
        screenTextInfo.buffer[i].hexColor = 0xFFFFFFFF;
    }

    screenTextInfo.index = 0;
    updateScreenTextBuffer();
}

void writeToVideoTextBuffer(const char* data, uint32_t data_len, uint32_t hexColor) {
    uint32_t i;

    for (i = 0; i < data_len; i++) {
        uint32_t pos = (i + screenTextInfo.index) % SCREEN_TEXT_BUFFER_SIZE;
        screenTextInfo.buffer[pos].c = data[i];
        screenTextInfo.buffer[pos].hexColor = hexColor;
    }

    screenTextInfo.index += i;
    screenTextInfo.index %= SCREEN_TEXT_BUFFER_SIZE;

    updateScreenTextBuffer();
}

void updateScreenTextBuffer() {
    clearScreen(0);
    uint32_t fontWidth = screenTextInfo.fontSize * (CHAR_BIT_WIDTH + CHAR_BIT_X_SPACING);
    uint32_t fontHeight = screenTextInfo.fontSize * (CHAR_BIT_HEIGHT + CHAR_BIT_Y_SPACING);
    uint32_t charsPerLine = VBE_mode_info->width / fontWidth;

    for (uint32_t i = 0; i < SCREEN_TEXT_BUFFER_SIZE; i++) {
        uint32_t posX = i % charsPerLine;
        uint32_t posY = i / charsPerLine;

        if (posY * fontHeight >= VBE_mode_info->height - CHAR_BIT_HEIGHT) break;

        for (uint32_t y = 0; y < CHAR_BIT_HEIGHT; y++) {
            for (uint32_t x = 0; x < CHAR_BIT_WIDTH; x++) {
                uint8_t bit = font_bitmap[screenTextInfo.buffer[i].c * CHAR_BIT_HEIGHT + y] & (0x80 >> x);
                if (bit) 
                    drawSquare(screenTextInfo.buffer[i].hexColor, posX * fontWidth + x * screenTextInfo.fontSize, posY * fontHeight + y * screenTextInfo.fontSize, screenTextInfo.fontSize);
            }
        }
    }
}
