#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <def.h>
#include <killer.h>
#include <lib.h>
#include <managers/kernel_managers.h>
#include <managers/processManager.h>
#include <managers/semaphoreManager.h>
#include <registers.h>
#include <stdint.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define MAX_LEN_BUFFER 256
#define CAPS_OFFSET 84
#define CAPS_LOCK_CODE_PRESSED 0x3A
#define LEFT_SHIFT_CODE_PRESSED 0x36
#define RIGHT_SHIFT_CODE_PRESSED 0x2A
#define LEFT_SHIFT_CODE_RELEASED 0xAA
#define RIGHT_SHIFT_CODE_RELEASED 0xB6
#define CNTRL_CODE_PRESSED 0x1D
#define CNTRL_CODE_RELEASED 0x9D
#define ESCAPE_CODE_PRESSED 0x01

int initialize_keyboard();
void keyboard_handler(processManagerADT process_manager,
                      semaphoreManagerADT semaphore_manager,
                      const registers64_t *registers);
uint8_t get_key_pending(int wait);
char get_character_pending(int wait);
uint8_t get_scan_code();
void set_input_mode(int mode);

void flush();
#endif
