#ifndef __ELIMINATOR__H__
#define __ELIMINATOR__H__

#include <std.h>
#include <stddef.h>
#include <stdint.h>
#include <syscall_adapters.h>

#define SQUARE_SIZE 10
#define MAX_LENGTH_PLAYER_NAME 20
#define MAX_DIGITS_SCORE 3
#define STARTING_HEIGHT 5
#define MAX_DELAY 100

#define UP_DIRECTION 0
#define RIGHT_DIRECTION 1
#define DOWN_DIRECTION 2
#define LEFT_DIRECTION 3

#define UP_ARROW_CODE_PRESSED 0x48
#define DOWN_ARROW_CODE_PRESSED 0x50
#define LEFT_ARROW_CODE_PRESSED 0x4B
#define RIGHT_ARROW_CODE_PRESSED 0x4D

#define W_KEY_CODE_PRESSED 0x11
#define A_KEY_CODE_PRESSED 0x1E
#define S_KEY_CODE_PRESSED 0x1F
#define D_KEY_CODE_PRESSED 0x20

#define HEX_WHITE 0x00FFFFFF
#define HEX_RED 0x00FF0000
#define HEX_GREEN 0x0000FF00

typedef struct {
  char name[MAX_LENGTH_PLAYER_NAME];
  uint8_t name_len;
  uint32_t x;
  uint32_t y;
  uint8_t direction;
  uint32_t color;
  uint8_t score;
  uint8_t score_digits;
  uint8_t alive;
} PlayerData;

uint64_t eliminator(char **argv, int argc);
void play();
void change_settings_and_play();
void print_setting(char *text, uint8_t max_text_size, uint8_t value,
                   uint32_t hexColor, uint32_t posX, uint32_t posY);
void scan_player_name(char *text, int len, uint8_t player_number,
                      PlayerData *player);
int endGame();

#endif