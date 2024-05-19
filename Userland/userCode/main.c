#include <syscall_adapters.h>
#include <std.h>

int main() {
    sys_set_font_size(1); 

    for (uint32_t i = 0; i < 50; i++)
        printf("count: %d\n", i);

    int a = 1;
    int b = 0;
    int c = a / b;

	return 0xDEADBEEF;
}
