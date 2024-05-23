#include <syscall_adapters.h>
#include <shell.h>
#include <std.h>

typedef struct {
    char* module_name;
    char* module_description;
    void (*module)();
} ModuleDescriptor;

ModuleDescriptor modules[] = {
    {"help", "displays available modules", help}, 
    {"clear", "clears the screens text buffer", cls}, 
    {"sysinfo", "displays system information", sys_info},
    {"regs", "displays stored register values", sys_print_registers},
    {"beep", "beeps", beep},
    {"div 0", "MUST REMOVE", div},
    };

void run_shell() {
    char shell_input[MAX_SHELL_INPUT];
    shell_input[0] = 0;

    while (1) { 
        puts("shell> ");
        scanf("%s", shell_input);
        for (uint32_t i = 0; i < sizeof(modules) / sizeof(modules[0]); i++) 
            if (strcmp(shell_input, modules[i].module_name) == 0)
                modules[i].module();
    }
}

void help() {
    puts("HELP\n");
    puts("available modules:\n");

    for (uint32_t i = 0; i < sizeof(modules) / sizeof(modules[0]); i++)
        printf("\t- %s: %s\n", modules[i].module_name, modules[i].module_description);

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

void beep() {
    sys_beep(500, 100);
    sys_beep(2000, 100);
}

void regs(){
    sys_print_registers();
}

void div(){
    int a = 0;
    int b = 24 / a;
    printf("%d", b);
}
