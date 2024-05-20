#include <syscall_adapters.h>
#include <std.h>
#include <keyboard.h>

int main() {
    sys_set_font_size(2); 

    printf("screen width: %d, ", sys_get_screen_width());
    printf("screen height: %d\n", sys_get_screen_height());

    for (uint32_t i = 0; i < 10; i++)
        printf("count: %d\n", i);

    int a = 1;
    int b = 0;
    //int c = a / b;

    while(1){
        char c = get_pressed_character();
        if(c != 0){
            putchar(c);
        }
    }

	return 0xDEADBEEF;
}
