#include <syscall_adapters.h>
#include <std.h>
#include <shell.h>

static uint8_t show_initial_info = 1;

int main() {
    if (show_initial_info) {
        puts_with_color("[SYSTEM INFO]\n\n", 0x006fb6fb);
        info();
        puts_with_color("[HELP]\n\n", 0x006fb6fb);
        help();
        show_initial_info = 0;
    }

    run_shell();

	return 0xDEADBEEF;
}
