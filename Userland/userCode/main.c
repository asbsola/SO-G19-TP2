#include <syscall_adapters.h>
#include <std.h>

int main() {
    sys_set_font_size(2); 

    printf("screen width: %d, ", sys_get_screen_width());
    printf("screen height: %d\n", sys_get_screen_height());

    for (uint32_t i = 0; i < 10; i++)
        printf("count: %d\n", i);

    int a = 1;
    int b = 0;

    __asm__("movq $3, %rax");
    __asm__("movq $4, %rbx");
    __asm__("movq $5, %rcx");
    __asm__("movq $6, %rdx");
    __asm__("movq $7, %rsi");
    __asm__("movq $8, %rdi");
    __asm__("movq $9, %r8");
    __asm__("movq $10, %r9");
    __asm__("movq $11, %r10");
    __asm__("movq $12, %r11");
    __asm__("movq $13, %r12");
    __asm__("movq $14, %r13");
    __asm__("movq $15, %r14");
    __asm__("movq $16, %r15");
    int c = a / b;
    while(1){
        uint32_t num;
        puts("Enter num: ");
        scanf("%d", &num);
        printf("num: %d\n", num);
        sys_set_font_size(num); 
    }
    
    // while(1){
    //     char str[200];
    //     int len = sys_read(str, 200);
    //     str[len] = 0;
    //     puts(str);
    // }
    

	return 0xDEADBEEF;
}
