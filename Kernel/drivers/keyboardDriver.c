// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <drivers/keyboardDriver.h>
#include <managers/kernel_managers.h>

uint8_t key_buffer[MAX_LEN_BUFFER];
static int first_key_index = 0;
static int buffer_size = 0;
static int input_mode = NON_CANNONICAL;
static int cntrl_down = 0;
static int caps_enabled = 0;
static sem_t key_sem;
static int blocked = 0;

static char map_to_ascii[256] = {0, '~', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '/', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 's', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 's', '*', 'a', ' ', '+', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'n', 's', '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', 0, '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', '/', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 's', '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 's', '*', 'a', ' ', '+', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'n', 's', '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.'};

int initialize_keyboard() {
	if ((key_sem = sem_open(the_semaphore_manager, 0)) == -1) return -1;
	return 0;
}

void keyboard_handler(processManagerADT process_manager, semaphoreManagerADT semaphore_manager, const registers64_t* registers) {
	uint8_t scan_code = get_scan_code();
	if (scan_code == CAPS_LOCK_CODE_PRESSED || scan_code == LEFT_SHIFT_CODE_PRESSED || scan_code == RIGHT_SHIFT_CODE_PRESSED || scan_code == LEFT_SHIFT_CODE_RELEASED || scan_code == RIGHT_SHIFT_CODE_RELEASED) {
		caps_enabled = !caps_enabled;
		return;
	}
	if (scan_code == CNTRL_CODE_PRESSED) {
		cntrl_down = 1;
		return;
	}
	if (scan_code == CNTRL_CODE_RELEASED) {
		cntrl_down = 0;
		return;
	}
	if (cntrl_down && map_to_ascii[scan_code + caps_enabled * CAPS_OFFSET] == 'd' && input_mode == CANNONICAL) {
		flush();
		send_eof(the_pipes_manager, KEYBOARD_INPUT_FD);
		return;
	}
	if (cntrl_down && map_to_ascii[scan_code + caps_enabled * CAPS_OFFSET] == 'c') {
		char* argv[] = {"killer", NULL};
		create_process(process_manager, IDLE_PROCESS_PID, killer, argv, KEYBOARD_INPUT_FD, SCREEN_OUTPUT_FD);
		return;
	}
	if (cntrl_down && map_to_ascii[scan_code + caps_enabled * CAPS_OFFSET] == 'C') {
		char* argv[] = {"killer", "r", NULL};
		create_process(process_manager, IDLE_PROCESS_PID, killer, argv, KEYBOARD_INPUT_FD, SCREEN_OUTPUT_FD);
		return;
	}
	if (scan_code == ESCAPE_CODE_PRESSED) {
		save_registers(registers);
		return;
	}
	if (scan_code > 0x80) return;

	char c[2] = {map_to_ascii[scan_code + caps_enabled * CAPS_OFFSET], 0};
	if (c[0] == '\b') {
		if (--buffer_size < 0) {
			buffer_size = 0;
			return;
		}
		if (input_mode == CANNONICAL) {
			if (map_to_ascii[key_buffer[first_key_index + buffer_size]] == '\t')
				pipe_write(the_pipes_manager, SCREEN_OUTPUT_FD, "\b\b\b\b", 5);
			else
				pipe_write(the_pipes_manager, SCREEN_OUTPUT_FD, c, 2);
		}
		return;
	}

	key_buffer[(first_key_index + buffer_size) % MAX_LEN_BUFFER] = scan_code + caps_enabled * CAPS_OFFSET;
	buffer_size++;
	if (input_mode == NON_CANNONICAL) {
		if (blocked) {
			blocked = 0;
			sem_up(the_semaphore_manager, key_sem);
		}
	} else {
		pipe_write(the_pipes_manager, SCREEN_OUTPUT_FD, c, 2);
		if (c[0] == '\n') flush();
	}
}

void flush() {
	while (buffer_size > 0) {
		pipe_write(the_pipes_manager, KEYBOARD_INPUT_FD, (const char*)&map_to_ascii[key_buffer[first_key_index]], 1);
		first_key_index = (first_key_index + 1) % MAX_LEN_BUFFER;
		buffer_size--;
	}
	first_key_index = 0;
}

uint8_t get_key_pending(int wait) {
	if (buffer_size == 0) {
		if (!wait) return 0;
		blocked = 1;
		sem_down(the_semaphore_manager, key_sem);
	}
	uint8_t key = key_buffer[first_key_index];
	first_key_index = (first_key_index + 1) % MAX_LEN_BUFFER;
	buffer_size--;
	return key;
}

char get_character_pending(int wait) {
	return map_to_ascii[get_key_pending(wait)];
}

void set_input_mode(int mode) {
	buffer_size = 0;
	first_key_index = 0;
	input_mode = mode;
}
