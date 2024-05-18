#include <syscall_adapters.h>

int main() {
    sys_set_font_size(1); 

    char text[39] = "Desde user space mande este mensaje \2\n";
    sys_write(2, text, 38); 

    // __asm__ volatile ("ud2");

    int a = 1;
    int b = 0;
    int c = a / b;

	return 0xDEADBEEF;
}
