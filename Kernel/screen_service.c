
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <managers/kernel_managers.h>
#include <process.h>
#include <screen_service.h>

uint64_t screen_service(char **argv, int argc) {
	set_font_size(1);
	clear_video_text_buffer();

	while (1) {
		char buff[LINE_MAX_LEN];
		int len = 0;
		while (len < LINE_MAX_LEN && (len == 0 || buff[len - 1] != '\0')) {
			int inc = read_pipe(the_pipes_manager, get_stdin(the_process_manager, get_current_pid(the_scheduler)), buff + len, 1);
			if (inc == EOF || inc == 0) break;
			len += inc;
		}
		_cli();
		write_to_video_text_buffer(buff, len - 1, 0x00ffffff);
		_sti();
		yield();
	}
}
