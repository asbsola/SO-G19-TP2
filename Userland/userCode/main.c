#include <syscall_adapters.h>
#include <std.h>
#include <shell.h>

static uint8_t show_initial_info = 1;

int main() {
    sys_set_font_size(2); 

    if (show_initial_info) {
        sys_info();
        help();
        show_initial_info = 0;
    }

    run_shell();

	return 0xDEADBEEF;
}
