#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>

#define MAX_SIZE_KEY_BUFFER 100

void keyboard_handler();
int keys_pending();
uint8_t get_key_pending();
uint8_t get_scan_code();

#endif
