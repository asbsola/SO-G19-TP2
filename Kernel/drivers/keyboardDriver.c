// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <drivers/keyboardDriver.h>
#include <lib.h>
#include <killer.h>

uint8_t key_buffer[MAX_SIZE_KEY_BUFFER];
static int first_key_index = 0;
static int buffer_size = 0;
static int cntrl_down = 0;
static int caps_enabled = 0;
const int caps_offset = 84;
static char map_to_ascii[256] = {
    0, '~', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t', 
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '/', 
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 's', '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 's', '*', 'a', ' ', '+',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'n', 's', '7', '8', '9',
    '-', '4', '5', '6', '+', '1', '2', '3', '0', '.',
    0, '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t', 
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', '/', 
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 's', '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 's', '*', 'a', ' ', '+',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'n', 's', '7', '8', '9',
    '-', '4', '5', '6', '+', '1', '2', '3', '0', '.'
};

void keyboard_handler(processManagerADT process_manager, const registers64_t * registers){
    uint8_t scan_code = get_scan_code();
    if(scan_code == CAPS_LOCK_CODE_PRESSED ||
     scan_code == LEFT_SHIFT_CODE_PRESSED || scan_code == RIGHT_SHIFT_CODE_PRESSED ||
     scan_code == LEFT_SHIFT_CODE_RELEASED || scan_code == RIGHT_SHIFT_CODE_RELEASED){
        caps_enabled = !caps_enabled;
        return;
    }
    if(scan_code == CNTRL_CODE_PRESSED){
        cntrl_down = 1;
        return;
    }
    if(scan_code == CNTRL_CODE_RELEASED){
        cntrl_down = 0;
        return;
    }
    if(cntrl_down && map_to_ascii[scan_code + caps_enabled * caps_offset] == 'c'){
        cntrl_down = 0;
        char* argv[] = {"killer", NULL};
        create_process(process_manager, IDLE_PROCESS_PID, killer, argv);
        return;
    }
    if(cntrl_down && map_to_ascii[scan_code + caps_enabled * caps_offset] == 'C'){
        cntrl_down = 0;
        char* argv[] = {"killer", "r", NULL};
        create_process(process_manager, IDLE_PROCESS_PID, killer, argv);
        return;
    }
    if(scan_code == ESCAPE_CODE_PRESSED){
        save_registers(registers);
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
