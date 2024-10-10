#include <shell.h>
#include <def.h>

ModuleDescriptor modules[] = {
    {"help", "displays available modules", BUILT_IN, help},
    {"clear", "clears the screens text buffer", BUILT_IN, cls},
    {"sysinfo", "displays system information", BUILT_IN, info},
    {"fontsize", "change font size", BUILT_IN, font_size},
    {"time", "display current time", BUILT_IN, time},
    {"regs", "displays captured registers (ESC key to capture)", BUILT_IN, regs},
    {"beep", "beeps", BUILT_IN, beep},
    {"song", "plays a short tune while displaying graphics", PROCESS, song},
    {"calculator", "positive integer calculator", PROCESS, calculator},
    {"eliminator", "eliminator game", PROCESS, eliminator},
    {"jump", "jumps to address given by user in decimal (1407583 causes invalid opcode >:) )", PROCESS, jump},
    {"test_mm", "tests the memory manager", PROCESS, test_mm},
    {"test_processes", "tests processes", PROCESS, test_processes},
    {"mem", "displays memory status", BUILT_IN, mem},
    {"ps", "displays information about processes", BUILT_IN, ps}};

static int current_font_size = 1;

void run_shell()
{

    char shell_input[MAX_SHELL_INPUT];
    shell_input[0] = 0;

    while (strcmp(shell_input, "exit") != 0)
    {
        sys_set_font_size(current_font_size);
        puts_with_color("shell> ", 0x006fb5fb);
        scanf("%s", shell_input);
        for (uint32_t i = 0; i < sizeof(modules) / sizeof(modules[0]); i++) {
            if (strcmp(shell_input, modules[i].module_name) == 0) {
                if (modules[i].module_type == PROCESS) {
                    char* argv[] = {modules[i].module_name, NULL};
                    sys_create_process(NOT_IN_FOREGROUND, modules[i].module, argv);
                    sys_wait();
                }
                else if (modules[i].module_type == BUILT_IN) {
                    char* argv[] = {modules[i].module_name, NULL};
                    modules[i].module(argv, 1);
                }

                break;
            }
        }
    }
}

uint64_t help(char** argv, int argc)
{
    puts("available modules:\n");

    puts("\t- exit: exit to kernel.\n");

    for (uint32_t i = 0; i < sizeof(modules) / sizeof(modules[0]); i++)
        printf("\t- %s: %s.\n", modules[i].module_name, modules[i].module_description);

    putchar('\n');

    return 0;
}

uint64_t cls(char** argv, int argc)
{
    sys_clear_text_buffer();

    return 0;
}

uint64_t info(char** argv, int argc)
{
    puts("screen info:\n");
    printf("width: %d pixels\n", sys_get_screen_width());
    printf("height: %d pixels\n\n", sys_get_screen_height());

    char cpu_vendor_buff[200];
    sys_get_cpu_vendor(cpu_vendor_buff);
    printf("cpu vendor: %s\n\n", cpu_vendor_buff);

    return 0;
}

uint64_t font_size(char** argv, int argc)
{
    int n = 0;
    printf("Choose font size (1-5) or 0 to exit: ");
    scanf("%d", &n);
    while (n < 0 || n >= 6)
    {
        printf("Choose a valid font size (1-5) or 0 to exit: ");
        scanf("%d", &n);
    }
    if (n != 0)
    {
        current_font_size = n;
        printf("Font size set to %d\n", n);
    }

    return 0;
}

uint64_t time(char** argv, int argc)
{
    char *time = sys_get_time(-3);
    printf("%s\n", time);

    return 0;
}

uint64_t regs(char** argv, int argc)
{
    sys_print_registers();
    
    return 0;
}

uint64_t song(char** argv, int argc)
{
    play_la_cucaracha();

    return 0;
}

uint64_t beep(char** argv, int argc)
{
    sys_beep(500, 100);
    sys_beep(2000, 100);

    return 0;
}

uint64_t calculator(char** argv, int argc)
{
    uint32_t num1 = 0;
    uint32_t num2 = 0;
    uint32_t response = 0;

    char operator;

    printf("Enter only positive numbers.\n");

    printf("First number: ");
    scanf("%d", &num1);

    printf("Operator: ");
    scanf("%c", &operator);

    printf("Second number: ");
    scanf("%d", &num2);

    switch (operator)
    {
    case '+':
        response = num1 + num2;
        break;
    case '-':
        response = num1 - num2;
        break;
    case '*':
        response = num1 * num2;
        break;
    case '/':
        response = num1 / num2;
        break;
    default:
        printf("Invalid operator.\n");
        return -1;
    }

    printf("Result: %d\n", response);
    return 0;
}

uint64_t eliminator(char** argv, int argc)
{
    play_eliminator();

    return 0;
}

uint64_t jump(char** argv, int argc)
{
    uint64_t dir = 0;
    printf("Address: ");
    scanf("%d", &dir);
    printf("Jumping to address %d\n", dir);
    jump_to_dir(dir);

    return 0;
}

uint64_t mem(char** argv, int argc)
{
    printf("Total memory:    %d\n", sys_get_total_memory_size());
    printf("Usable memory:   %d\n", sys_get_usable_memory_size());
    printf("Free memory:     %d\n", sys_get_free_memory_size());
    printf("Occupied memory: %d\n", sys_get_usable_memory_size() - sys_get_free_memory_size());

    return 0;
}

const char* process_priority_names[] = {"LOW", "MEDIUM", "HIGH"};
const char* process_status_names[] = {"RUNNING", "READY", "BLOCKED", "EXITED", "KILLED"};

uint64_t ps(char** argv, int argc) {
    process_info_t * processes = (process_info_t *)sys_ps();
    for (int i = 0; processes[i].pid != -1; i++){
        printf("\nProcess %d\n", i);
        printf("\tPID: %d\n", processes[i].pid);

        if(processes[i].parent_pid == -1) printf("\tParent PID: -1\n");
        else printf("\tParent PID: %d\n", processes[i].parent_pid);

        printf("\tPriority: %s\n", process_priority_names[processes[i].priority]); 
        printf("\tStatus: %s\n", process_status_names[processes[i].status]);

        printf("\tForeground: %s IN FOREGROUND\n", (processes[i].is_in_foreground == IN_FOREGROUND) ? "" : "NOT");
        printf("\tStack pointer: %d\n", processes[i].stack_pointer);
        printf("\tBase pointer: %d\n", processes[i].base_pointer);
    }

    return 0;
}
