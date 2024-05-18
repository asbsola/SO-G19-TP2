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

void put_pixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    if (y > VBE_mode_info->height || x > VBE_mode_info->width)
        return;

    uint8_t * framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}

void draw_square(uint32_t hexColor, uint32_t posX, uint32_t posY, uint32_t size) {
    for (uint32_t y = posY; y < posY + size; y++)
        for (uint32_t x = posX; x < posX + size; x++)
            put_pixel(hexColor, x, y);
}

void clear_screen(uint32_t clearColor) {
    for (uint32_t y = 0; y < VBE_mode_info->height; y++)
        for (uint32_t x = 0; x < VBE_mode_info->width; x++)
            put_pixel(clearColor, x, y);
}

typedef struct {
    uint32_t hexColor;
    char c;
} ScreenTextChar;

struct ScreenTextInfo {
    uint32_t index;
    uint32_t fontSize;
    uint32_t lastStartPos;
    ScreenTextChar buffer[SCREEN_TEXT_BUFFER_SIZE];
} screenTextInfo;

uint32_t get_font_width() {
    return screenTextInfo.fontSize * (CHAR_BIT_WIDTH + CHAR_BIT_X_SPACING);
}

uint32_t get_font_height() {
    return screenTextInfo.fontSize * (CHAR_BIT_HEIGHT + CHAR_BIT_Y_SPACING);
}

uint32_t get_chars_per_line() {
    return VBE_mode_info->width / get_font_width();
}

void set_font_size(uint32_t fontSize) {
    if (fontSize > 0 && fontSize < 6) {
        screenTextInfo.fontSize = fontSize;
        update_screen_text_buffer();
    }
}


void draw_char(char c, uint32_t hexColor, uint32_t posX, uint32_t posY) {
    for (uint32_t y = 0; y < CHAR_BIT_HEIGHT; y++) {
        for (uint32_t x = 0; x < CHAR_BIT_WIDTH; x++) {
            uint8_t bit = font_bitmap[c * CHAR_BIT_HEIGHT + y] & (0x80 >> x);
            if (bit) 
                draw_square(hexColor, posX + x * screenTextInfo.fontSize, posY + y * screenTextInfo.fontSize, screenTextInfo.fontSize);
        }
    }
}

void draw_string(const char* str, uint32_t len, uint32_t hexColor, uint32_t posX, uint32_t posY) {
    uint32_t fontWidth = get_font_width();

    for (int i = 0; i < len; i++) draw_char(str[i], hexColor, posX + i * fontWidth, posY);
}

void clear_video_text_buffer() {
    for (uint32_t i = 0; i < SCREEN_TEXT_BUFFER_SIZE; i++) {
        screenTextInfo.buffer[i].c = ' ';
        screenTextInfo.buffer[i].hexColor = 0xFFFFFFFF;
    }

    screenTextInfo.index = 0;
    clear_screen(0);
}

void write_to_video_text_buffer(const char* data, uint32_t data_len, uint32_t hexColor) {
    uint32_t i;

    for (i = 0; i < data_len; i++) {
        uint32_t pos = (i + screenTextInfo.index) % SCREEN_TEXT_BUFFER_SIZE;
        screenTextInfo.buffer[pos].c = data[i];
        screenTextInfo.buffer[pos].hexColor = hexColor;
    }

    screenTextInfo.index += i;
    screenTextInfo.index %= SCREEN_TEXT_BUFFER_SIZE;

    update_screen_text_buffer();
}

uint32_t get_buffer_start_pos() {
    uint32_t fontWidth = screenTextInfo.fontSize * (CHAR_BIT_WIDTH + CHAR_BIT_X_SPACING);
    uint32_t fontHeight = screenTextInfo.fontSize * (CHAR_BIT_HEIGHT + CHAR_BIT_Y_SPACING);
    uint32_t charsPerLine = VBE_mode_info->width / fontWidth;

    uint32_t j = 0;
    uint32_t out = 0;

    for (uint32_t i = 0; i < screenTextInfo.index; i++) {
        uint32_t posX = j % charsPerLine;
        uint32_t posY = j / charsPerLine;

        if (posY * fontHeight >= VBE_mode_info->height) {
            out = i;
            j = 0;
        }
        else {
            j += (screenTextInfo.buffer[i].c == '\n') ? (charsPerLine - (j % charsPerLine)) : 1;
        }
    }

    return out;
}

void update_screen_text_buffer() {
    uint32_t fontWidth = get_font_width();
    uint32_t fontHeight = get_font_height();
    uint32_t charsPerLine = get_chars_per_line();

    uint32_t j = 0;

    uint32_t start_pos = get_buffer_start_pos();

    if (start_pos != screenTextInfo.lastStartPos) {
        clear_screen(0);
        screenTextInfo.lastStartPos = start_pos;
    }

    for (uint32_t i = start_pos; i < screenTextInfo.index; i++) {
        uint32_t posX = j % charsPerLine;
        uint32_t posY = j / charsPerLine;

        if (posY * fontHeight >= VBE_mode_info->height) return;

        if (screenTextInfo.buffer[i].c == '\n') {
            j += charsPerLine - posX;
        }
        else {
            draw_char(screenTextInfo.buffer[i].c, screenTextInfo.buffer[i].hexColor, posX * fontWidth, posY * fontHeight);
            j++;
        }
    }
}
