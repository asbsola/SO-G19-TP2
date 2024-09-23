#include <stdint.h>
#include <drivers/videoDriver.h>
#include <lib.h>
#include <interruptHandlers/interrupts.h>

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

uint8_t video_buffer[MAX_VIDEO_BUFFER_WIDTH * MAX_VIDEO_BUFFER_HEIGHT * MAX_VIDEO_BUFFER_BYTES_PER_PIXEL];

uint32_t get_video_buffer_width() {
    return (MAX_VIDEO_BUFFER_WIDTH > VBE_mode_info->width) ? VBE_mode_info->width : MAX_VIDEO_BUFFER_WIDTH;
}

uint32_t get_video_buffer_height() {
    return (MAX_VIDEO_BUFFER_HEIGHT > VBE_mode_info->height) ? VBE_mode_info->height : MAX_VIDEO_BUFFER_HEIGHT;
}

uint32_t get_video_buffer_bytes_per_pixel() {
    uint32_t bytesPerPixel = VBE_mode_info->bpp / 8;
    return (MAX_VIDEO_BUFFER_BYTES_PER_PIXEL > bytesPerPixel) ? bytesPerPixel : MAX_VIDEO_BUFFER_BYTES_PER_PIXEL;
}

void update_frame_buffer() {
    memcpy(VBE_mode_info->framebuffer, video_buffer, get_video_buffer_width() * get_video_buffer_height() * get_video_buffer_bytes_per_pixel());
}

void put_pixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    if (y > get_video_buffer_height() || x > get_video_buffer_width())
        return;

    uint64_t offset = (x * get_video_buffer_bytes_per_pixel()) + (y * VBE_mode_info->pitch);
    video_buffer[offset]     =  (hexColor) & 0xFF;
    video_buffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    video_buffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}

void draw_rect(uint32_t hexColor, uint32_t posX, uint32_t posY, uint32_t width, uint32_t height) {
    if (posX + width > get_video_buffer_width() || posY + height > get_video_buffer_height())
        return;

    uint8_t r = (hexColor) & 0xFF;
    uint8_t g = (hexColor >> 8) & 0xFF;
    uint8_t b = (hexColor >> 16) & 0xFF;

    uint32_t bytesPerPixel = get_video_buffer_bytes_per_pixel();

    for (uint32_t y = posY; y < posY + height; y++) {
        for (uint32_t x = posX; x < posX + width; x++) {
            uint64_t offset = (x * bytesPerPixel) + (y * VBE_mode_info->pitch);
            video_buffer[offset]     =  r;
            video_buffer[offset+1]   =  g; 
            video_buffer[offset+2]   =  b;
        }
    }
}

void draw_square(uint32_t hexColor, uint32_t posX, uint32_t posY, uint32_t size) {
    draw_rect(hexColor, posX, posY, size, size);
}

void clear_screen(uint32_t clearColor) {
    draw_rect(clearColor, 0, 0, get_video_buffer_width(), get_video_buffer_height());
}

typedef struct {
    uint32_t hexColor;
    char c;
} ScreenTextChar;

struct ScreenTextInfo {
    uint32_t indexX;
    uint32_t indexY;
    uint32_t fontSize;
    ScreenTextChar buffer[SCREEN_TEXT_BUFFER_HEIGHT][SCREEN_TEXT_BUFFER_WIDTH];
} screenTextInfo;

uint32_t get_font_width() {
    return screenTextInfo.fontSize * (CHAR_BIT_WIDTH + CHAR_BIT_X_SPACING);
}

uint32_t get_font_height() {
    return screenTextInfo.fontSize * (CHAR_BIT_HEIGHT + CHAR_BIT_Y_SPACING);
}

uint32_t get_chars_per_line() {
    return get_video_buffer_width() / get_font_width();
}

uint32_t get_chars_per_buff_line() {
    uint32_t charsPerLine = get_chars_per_line();
    return (charsPerLine <= SCREEN_TEXT_BUFFER_WIDTH) ? charsPerLine : SCREEN_TEXT_BUFFER_WIDTH;
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
            uint8_t bit = font_bitmap[c * CHAR_BIT_HEIGHT + y] & (1 << (CHAR_BIT_WIDTH - x));
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
    for (uint32_t y = 0; y < SCREEN_TEXT_BUFFER_HEIGHT; y++) {
        for (uint32_t x = 0; x < SCREEN_TEXT_BUFFER_WIDTH; x++) {
            screenTextInfo.buffer[y][x].c = ' ';
            screenTextInfo.buffer[y][x].hexColor = 0xFFFFFFFF;
        }
    }

    screenTextInfo.indexX = 0;
    screenTextInfo.indexY = 0;
    clear_screen(0);
}

void write_to_video_text_buffer(const char* data, uint32_t data_len, uint32_t hexColor) {
    for (uint32_t i = 0; i < data_len; i++) {
        switch (data[i]) {
            case '\n':
                screenTextInfo.indexY += 1;

                if (screenTextInfo.indexY == SCREEN_TEXT_BUFFER_HEIGHT) clear_video_text_buffer(); 
                screenTextInfo.indexY %= SCREEN_TEXT_BUFFER_HEIGHT;

                screenTextInfo.indexX = 0;
                break;
            case '\t':
                write_to_video_text_buffer("    ", 4, hexColor);
                break;
            case '\r':
                screenTextInfo.indexX = 0;
                break;
            case '\b':
                if (screenTextInfo.indexX > 0) {
                    screenTextInfo.indexX -= 1;
                    screenTextInfo.buffer[screenTextInfo.indexY][screenTextInfo.indexX].c = ' ';
                } else if (screenTextInfo.indexY > 0) {
                    screenTextInfo.indexX = get_chars_per_buff_line() - 1;
                    screenTextInfo.indexY -= 1;
                    screenTextInfo.buffer[screenTextInfo.indexY][screenTextInfo.indexX].c = ' ';
                }
                break;
            default:
                screenTextInfo.buffer[screenTextInfo.indexY][screenTextInfo.indexX].c = data[i];
                screenTextInfo.buffer[screenTextInfo.indexY][screenTextInfo.indexX].hexColor = hexColor;

                screenTextInfo.indexX += 1;
                screenTextInfo.indexY += screenTextInfo.indexX / get_chars_per_buff_line();
                screenTextInfo.indexX %= get_chars_per_buff_line();
                break;
        }
    }

    update_screen_text_buffer();
}

void update_screen_text_buffer() {
    _cli();
    clear_screen(0);

    uint32_t fontWidth = get_font_width();
    uint32_t fontHeight = get_font_height();
    uint32_t linesPerScreen = get_video_buffer_height() / fontHeight - 1;

    uint32_t lower_y_limit = (linesPerScreen <= screenTextInfo.indexY) ? screenTextInfo.indexY  - linesPerScreen : 0;
    uint32_t upper_x_limit = get_chars_per_buff_line();

    for (uint32_t y = screenTextInfo.indexY; y + 1 > lower_y_limit; y--)
        for (uint32_t x = 0; x < upper_x_limit; x++)
            draw_char(screenTextInfo.buffer[y][x].c, screenTextInfo.buffer[y][x].hexColor, x * fontWidth, (y - lower_y_limit) * fontHeight);
    _sti();
}
