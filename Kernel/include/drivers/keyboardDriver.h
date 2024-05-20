#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>

#define CAPS_LOCK_CODE 0x3A
#define LEFT_SHIFT_CODE 0x36
#define RIGHT_SHIFT_CODE 0x2A

#define MAX_SIZE_KEY_BUFFER 100

void keyboard_handler();
int keys_pending();
uint8_t get_key_pending();
uint8_t get_scan_code();
char get_pressed_character();

#endif
