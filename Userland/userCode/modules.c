#include <syscall_adapters.h>
#include <std.h>

void help() {
    puts("HELP\n");
    puts("available modules:\n");
    puts("- help: displays available modules.\n");
    puts("- clear: clears the screens text buffer.\n");
    puts("- sysinfo: displays system information.\n");
    putchar('\n');
}

void cls() {
    sys_clear_text_buffer();
}

void sys_info() {
    puts("SYSTEM INFO\n");

    puts("screen info:\n");
    printf("width: %d pixels\n", sys_get_screen_width());
    printf("height: %d pixels\n\n", sys_get_screen_height());

    char cpu_vendor_buff[200];
    sys_get_cpu_vendor(cpu_vendor_buff);
    printf("cpu vendor: %s\n\n", cpu_vendor_buff);
}
