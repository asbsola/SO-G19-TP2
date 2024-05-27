#include <syscall_adapters.h>
#include <shell.h>
#include <std.h>
#include <cucaracha.h>
#include <eliminator.h>

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
    {"calculator", "positive integer calculator", calculator},
    {"eliminator", "eliminator game", eliminator}
    };

static int current_font_size = 1;

void run_shell() {

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

void calculator(){
    int num1 = -1;
    int num2;
    float response;
    char operator[2];

    printf("Only positive numbers.\n");

    printf("First number: ");
    scanf("%d", &num1);
    
    printf("Second number: ");
    scanf("%d", &num2);

    printf("Operator: ");
    scanf("%c", &operator);


    if (strcmp(operator, "+") == 0) {
        response = num1 + num2;
    } else if (strcmp(operator, "-") == 0) {
        response = num1 - num2;
    } else if (strcmp(operator, "*") == 0) {
        response = num1 * num2;
    } else if (strcmp(operator, "/") == 0) {
        response = num1 / num2;
    } else {
        printf("Invalid operator.\n");
        return;
    }
    
    char stringResponse[11];

    puts("Result: ");
    puts(itoa(response, stringResponse, 10));
    puts("\n");
}

void eliminator() {
    play_eliminator();
}
