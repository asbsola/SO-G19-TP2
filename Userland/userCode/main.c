#include <syscall_adapters.h>

int main() {
    sys_set_font_size(1); 

    char text[39] = "Desde user space mande este mensaje \2\n";
    sys_write(2, text, 38); 
	return 0xDEADBEEF;
}
