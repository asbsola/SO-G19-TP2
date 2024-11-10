// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <programs/cucaracha.h>

typedef struct {
  uint32_t freq;
  uint32_t duration;
  uint32_t delay;
} note_t;

note_t notes[] = {
    {262, 150, 75},  {262, 150, 75},  {262, 150, 75},  {349, 250, 75},
    {440, 150, 150}, {262, 150, 75},  {262, 150, 75},  {262, 150, 75},
    {349, 250, 75},  {440, 150, 150}, {349, 250, 75},  {349, 150, 75},
    {330, 150, 75},  {330, 150, 75},  {294, 150, 75},  {294, 150, 75},
    {262, 150, 200}, {262, 150, 75},  {262, 150, 75},  {262, 150, 75},
    {330, 250, 75},  {392, 150, 150}, {262, 150, 75},  {262, 150, 75},
    {262, 150, 75},  {330, 250, 75},  {392, 150, 150}, {523, 250, 75},
    {587, 150, 75},  {523, 150, 75},  {494, 150, 75},  {440, 150, 75},
    {392, 150, 75},  {349, 150, 200}};

const uint32_t scale = 10;

uint64_t cucaracha(char **argv, int argc) {
  sys_clear_screen(0);

  const uint32_t frame_count = sizeof(cucaracha_gif) / sizeof(cucaracha_gif[0]);
  const uint32_t song_length = sizeof(notes) / sizeof(notes[0]);

  uint32_t frame = 0;
  uint32_t startXPos =
      (sys_get_screen_width() - CUCARACHA_GIF_WIDTH * scale) / 2;
  uint32_t startYPos =
      (sys_get_screen_height() - CUCARACHA_GIF_HEIGHT * scale) / 2;
  for (uint32_t i = 0; i < song_length; i++, frame++) {
    frame %= frame_count;

    for (uint32_t y = 0; y < CUCARACHA_GIF_HEIGHT; y++) {
      for (uint32_t x = 0; x < CUCARACHA_GIF_WIDTH; x++) {
        uint8_t col = cucaracha_gif[frame][x + CUCARACHA_GIF_WIDTH * y];
        uint32_t blue = (col & 3) * 64;
        uint32_t green = ((col & 28) >> 2) * 32;
        uint32_t red = (col >> 5) * 32;
        uint32_t hex_color = blue + (green << 8) + (red << 16);
        sys_draw_square(hex_color, startXPos + x * scale, startYPos + y * scale,
                        scale);
      }
    }

    sys_beep(notes[i].freq, notes[i].duration);
    sleep(notes[i].delay);
  }
  return 0;
}
