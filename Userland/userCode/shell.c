#include <syscall_adapters.h>
#include <shell.h>
#include <std.h>
#include <cucaracha.h>

typedef struct {
    char* module_name;
    char* module_description;
    void (*module)();
} ModuleDescriptor;

ModuleDescriptor modules[] = {
    {"help", "displays available modules", help}, 
    {"clear", "clears the screens text buffer", cls}, 
    {"info", "displays system information", info},
    {"size", "change font size", font_size},
    {"time", "display current time", time},
    {"regs", "displays captured registers (ESC key to capture)", regs},
    {"beep", "beeps", beep},
    {"song", "plays a short tune while displaying graphics", song},
    {"div 0", "MUST REMOVE", div},
    {"eliminator", "eliminator game", eliminator}
    };

static int current_font_size = 1;

void run_shell() {

    eliminator();

    char shell_input[MAX_SHELL_INPUT];
    shell_input[0] = 0;

    while (1) {
        sys_set_font_size(current_font_size);
        puts_with_color("shell> ", 0x006fb5fb);
        scanf("%s", shell_input);
        for (uint32_t i = 0; i < sizeof(modules) / sizeof(modules[0]); i++) 
            if (strcmp(shell_input, modules[i].module_name) == 0)
                modules[i].module();
    }
}

void help() {
    puts("available modules:\n");

    for (uint32_t i = 0; i < sizeof(modules) / sizeof(modules[0]); i++)
        printf("\t- %s: %s.\n", modules[i].module_name, modules[i].module_description);

    putchar('\n');
}

void cls() {
    sys_clear_text_buffer();
}

void info() {
    puts("screen info:\n");
    printf("width: %d pixels\n", sys_get_screen_width());
    printf("height: %d pixels\n\n", sys_get_screen_height());

    char cpu_vendor_buff[200];
    sys_get_cpu_vendor(cpu_vendor_buff);
    printf("cpu vendor: %s\n\n", cpu_vendor_buff);
}

void font_size(){
    int n = 0;
    printf("Choose font size (1-5) or 0 to exit: ");
    scanf("%d", &n);
    while(n < 0 || n >= 6){
        printf("Choose a valid font size (1-5) or 0 to exit: ");
        scanf("%d", &n);
    }
    if(n != 0){
        current_font_size = n;
        printf("Font size set to %d\n", n);
    }
}

void time(){
    char * time = sys_get_time(-3);
    printf("%s\n", time);
}

void regs(){
    sys_print_registers();
}

void song(){
    play_la_cucaracha();
}

void beep() {
    sys_beep(500, 100);
    sys_beep(2000, 100);
}

void div(){
    int a = 0;
    int b = 24 / a;
    printf("%d", b);
}

void eliminator() {

    uint32_t square_size = 10;

    uint32_t screen_width = sys_get_screen_width();
    uint32_t screen_height = sys_get_screen_height();

    uint32_t buffer[screen_height][screen_width];

    sys_clear_screen(0);

    uint32_t x = square_size;
    uint32_t y = square_size;
    while(x <= screen_width - square_size * 2){
        sys_draw_square(0x00FF0000, x, square_size, square_size);
        sys_draw_square(0x00FF0000, x, screen_height - square_size * 2, square_size);
        // sys_delay(30);
        x+=square_size;
    }

    while(y <= screen_height - square_size * 2){
        sys_draw_square(0x00FF0000, square_size, y, square_size);
        sys_draw_square(0x00FF0000, screen_width - square_size * 2, y, square_size);
        // sys_delay(30);
        y+=square_size;
    }

    while(1);

}
