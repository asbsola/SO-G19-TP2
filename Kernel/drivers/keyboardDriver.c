#include <drivers/keyboardDriver.h>
#include <lib.h>

uint8_t key_buffer[MAX_SIZE_KEY_BUFFER];
static int first_key_index = 0;
static int buffer_size = 0;

void keyboard_handler(){
    uint8_t scan_code = get_scan_code();
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
