// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <syscall_adapters.h>
#include <std.h>
#include <shell.h>

static uint8_t show_initial_info = 1;

int main() {
    if (show_initial_info) {
        puts_with_color("[SYSTEM INFO]\n\n", 0x006fb6fb);
        char* argv_info[] = {"info", NULL};
        info(argv_info, 1);

        puts_with_color("[HELP]\n\n", 0x006fb6fb);
        char* argv_help[] = {"help", NULL};
        help(argv_help, 1);
        show_initial_info = 0;
    }

    run_shell();

	return 0xDEADBEEF;
}
