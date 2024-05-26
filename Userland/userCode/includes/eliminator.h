#ifndef __eliminator__h__
#define __eliminator__h__

#include <stdint.h>
#include <syscall_adapters.h>
#include <std.h>

#define SQUARE_SIZE 10
#define MAX_PLAYER_NAME 20

#define UP_ARROW_CODE_PRESSED 0x48
#define DOWN_ARROW_CODE_PRESSED 0x50
#define LEFT_ARROW_CODE_PRESSED 0x4B
#define RIGHT_ARROW_CODE_PRESSED 0x4D

#define W_KEY_CODE_PRESSED 0x11
#define A_KEY_CODE_PRESSED 0x1E
#define S_KEY_CODE_PRESSED 0x1F
#define D_KEY_CODE_PRESSED 0x20

void play_eliminator();

#endif