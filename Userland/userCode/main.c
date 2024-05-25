#include <syscall_adapters.h>
#include <std.h>
#include <shell.h>

static uint8_t show_initial_info = 1;

int main() {

    if (show_initial_info) {
        puts("[SYSTEM INFO]\n\n");
        info();
        puts("[HELP]\n\n");
        help();
        show_initial_info = 0;
    }

    run_shell();

	return 0xDEADBEEF;
}
