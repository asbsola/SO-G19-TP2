#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>
#include <managers/processManager.h>
#include <registers.h>

#define CAPS_LOCK_CODE_PRESSED 0x3A
#define LEFT_SHIFT_CODE_PRESSED 0x36
#define RIGHT_SHIFT_CODE_PRESSED 0x2A
#define LEFT_SHIFT_CODE_RELEASED 0xAA
#define RIGHT_SHIFT_CODE_RELEASED 0xB6
#define CNTRL_CODE_PRESSED 0x1D
#define CNTRL_CODE_RELEASED 0x9D
#define ESCAPE_CODE_PRESSED 0x01

#define MAX_SIZE_KEY_BUFFER 100

void keyboard_handler(processManagerADT process_manager, const registers64_t * registers);
int keys_pending();
uint8_t get_key_pending();
uint8_t get_scan_code();
char get_pressed_character();

#endif
