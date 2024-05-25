#include <cucaracha.h>
#include <syscall_adapters.h>
#include <std.h>

typedef struct{
    uint32_t freq;
    uint32_t duration;
    uint32_t delay;
} note_t;

const int song_length = 34;
note_t notes[] = {
    {262, 200, 100}, {262, 200, 100}, {262, 200, 100}, {349, 400, 100}, {440, 200, 200},
    {262, 200, 100}, {262, 200, 100}, {262, 200, 100}, {349, 400, 100}, {440, 200, 200},
    {349, 400, 100}, {349, 200, 100}, {330, 200, 100}, {330, 200, 100}, {294, 200, 100}, {294, 200, 100}, {262, 200, 300},
    {262, 200, 100}, {262, 200, 100}, {262, 200, 100}, {330, 400, 100}, {392, 200, 200},
    {262, 200, 100}, {262, 200, 100}, {262, 200, 100}, {330, 400, 100}, {392, 200, 200},
    {523, 400, 100}, {587, 200, 100}, {523, 200, 100}, {494, 200, 100}, {440, 200, 100}, {392, 200, 100}, {349, 200, 300}
};

const uint32_t scale = 10;

void play_la_cucaracha(){
    sys_clear_screen(0);

    const uint32_t frameCount = sizeof(cucaracha_gif) / sizeof(cucaracha_gif[0]);
    
    uint32_t frame = 0;
    for (uint32_t i = 0; i < song_length; i++, frame++) {
        frame %= frameCount;

        for (uint32_t y = 0; y < CUCARACHA_GIF_HEIGHT; y++) {
            for (uint32_t x = 0; x < CUCARACHA_GIF_WIDTH; x++) {
                uint8_t col = cucaracha_gif[frame][x + CUCARACHA_GIF_WIDTH * y];
                uint32_t blue = (col & 3) * 64;
                uint32_t green = ((col & 28) >> 2) * 32;
                uint32_t red = (col >> 5) * 32;
                uint32_t hexColor = blue + (green << 8) + (red << 16);
                sys_draw_square(hexColor, x * scale, y * scale, scale);
            }
        }

        sys_beep(notes[i].freq, notes[i].duration);
        sys_delay(notes[i].delay);
    }
}