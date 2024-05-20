#include <drivers/keyboardDriver.h>
#include <registers.h>
#include <lib.h>

uint8_t key_buffer[MAX_SIZE_KEY_BUFFER];
static int first_key_index = 0;
static int buffer_size = 0;
static int caps_enabled = 0;
const int caps_offset = 84;
static char map_to_ascii[256] = {
    0, '~', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t', 
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 'c', 
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 's', '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 's', '*', 'a', ' ', 'c',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'n', 's', '7', '8', '9',
    '-', '4', '5', '6', '+', '1', '2', '3', '0', '.',
    0, '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t', 
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 'c', 
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 's', '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 's', '*', 'a', ' ', 'c',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'n', 's', '7', '8', '9',
    '-', '4', '5', '6', '+', '1', '2', '3', '0', '.'
};

void keyboard_handler(const registers64_t * registers){
    uint8_t scan_code = get_scan_code();
    if(scan_code == CAPS_LOCK_CODE || scan_code == LEFT_SHIFT_CODE || scan_code == RIGHT_SHIFT_CODE){
        caps_enabled = 1 - caps_enabled;
        return;
    }
    if(scan_code > 0x80 || buffer_size >= MAX_SIZE_KEY_BUFFER) return;
    key_buffer[(first_key_index + buffer_size++) % MAX_SIZE_KEY_BUFFER] = scan_code;
}

int keys_pending(){
    return buffer_size > 0;
}

uint8_t get_key_pending(){
    if(!keys_pending()) return 0;
    uint8_t key = key_buffer[first_key_index];
    first_key_index = (first_key_index + 1) % MAX_SIZE_KEY_BUFFER;
    buffer_size--;
    return key;
}

char get_pressed_character(){
    return map_to_ascii[get_key_pending() + caps_enabled * caps_offset];
}