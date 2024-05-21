#include <syscall_adapters.h>
#include <std.h>
#include <modules.h>

#define MAX_SHELL_INPUT 1024

typedef struct {
    char* module_name;
    void (*module)();
} ModuleDescriptor;

void divby0() {
    int a = 4;
    int b = 0;
    int c = a / b;
}

ModuleDescriptor modules[] = {{"help", help}, {"clear", cls}, {"sysinfo", sys_info}, {"divby0", divby0}};

void run_shell() {
    char shell_input[MAX_SHELL_INPUT];
    shell_input[0] = 0;

    while (strcmp(shell_input, "exit") != 0) { 
        puts("shell> ");
        scanf("%s", shell_input);
        for (uint32_t i = 0; i < sizeof(modules) / sizeof(modules[0]); i++) 
            if (strcmp(shell_input, modules[i].module_name) == 0)
                modules[i].module();
    }
}

static uint8_t show_initial_info = 1;

int main() {
    sys_set_font_size(2); 

    if (show_initial_info) {
        sys_info();
        help();
        show_initial_info = 0;
    }

    run_shell();

	return 0xDEADBEEF;
}
