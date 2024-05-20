#include <syscall_adapters.h>
#include <std.h>

int main() {
    sys_set_font_size(2); 

    printf("screen width: %d, ", sys_get_screen_width());
    printf("screen height: %d\n", sys_get_screen_height());

    for (uint32_t i = 0; i < 100; i++)
        printf("count: %d\n", i);

    int a = 1;
    int b = 0;
    //int c = a / b;

    while(1){
        char str[200];
        int len = sys_read(str, 200);
        str[len] = 0;
        puts(str);
    }

	return 0xDEADBEEF;
}
