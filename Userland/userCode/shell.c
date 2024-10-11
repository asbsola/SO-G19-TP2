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
    {"jump", "jumps to address given by user in decimal (12345 causes invalid opcode)", PROCESS, jump},
    {"test_mm", "tests the memory manager", PROCESS, test_mm},
    {"test_processes", "tests processes", PROCESS, test_processes},
    {"test_priority", "tests priority", PROCESS, test_prio},
    {"mem", "displays memory status", BUILT_IN, mem},
    {"ps", "displays information about processes", BUILT_IN, ps},
    {"nicent", "changes process priority by PID", BUILT_IN, nicent},
    {"kill", "terminates a process by its PID", BUILT_IN, kill}};

static int current_font_size = 1;

char* get_last_arg(char** args);

void run_shell()
{

    char shell_input[MAX_SHELL_INPUT];
    shell_input[0] = 0;
    sys_nicent(sys_get_pid(), HIGH);
    
    while (strcmp(shell_input, "exit") != 0)
    {
        sys_set_font_size(current_font_size);
        puts_with_color("shell> ", 0x006fb5fb);
        scanf("%s", shell_input);

        int argc = 0;
        char** argv = get_args(shell_input, &argc);
        char* last_arg = get_last_arg(argv);
        uint8_t executed_command = 0;

        uint8_t in_background = (last_arg != NULL && last_arg[0] == '&');

        for (uint32_t i = 0; i < sizeof(modules) / sizeof(modules[0]); i++) {
            if (strcmp(argv[0], modules[i].module_name) == 0) {
                if (modules[i].module_type == PROCESS) {
                    executed_command = 1;
                    sys_create_process((in_background) ? NOT_IN_FOREGROUND : IN_FOREGROUND, modules[i].module, argv);
                    if (!in_background) sys_wait();
                }
                else if (modules[i].module_type == BUILT_IN) {
                    executed_command = 1;
                    modules[i].module(argv, argc);
                }

                break;
            }
        }

        if (!executed_command) 
            printf("Unknown command: %s\n", argv[0]);

        free_args(argv);
    }
}

uint64_t help(char** argv, int argc)
{

    puts_with_color("arguments are separated by ' ' (spaces).\nadd & at the last argument of a command to run in background (only for processes).\n\n", 0xc2daff);

    puts("available modules:\n");

    puts("\t- exit: exit to kernel.\n");

    for (uint32_t i = 0; i < sizeof(modules) / sizeof(modules[0]); i++) {
        printf("\t- %s - ", (modules[i].module_type == BUILT_IN) ? "built-in" : "process "); 
        puts_with_color(modules[i].module_name, 0x006fb5fb);
        printf(": %s.\n", modules[i].module_description);
    }

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
        printf("\tName: %s\n", processes[i].name);

        if(processes[i].parent_pid == -1) printf("\tParent PID: -1\n");
        else printf("\tParent PID: %d\n", processes[i].parent_pid);

        printf("\tPriority: %s\n", process_priority_names[processes[i].priority]); 
        printf("\tStatus: %s\n", process_status_names[processes[i].status]);

        printf("\tForeground: %s IN FOREGROUND\n", (processes[i].is_in_foreground == IN_FOREGROUND) ? "" : "NOT");
        printf("\tStack pointer: %d\n", processes[i].stack_pointer);
        printf("\tBase pointer: %d\n", processes[i].base_pointer);
    }

    sys_free(processes); 

    return 0;
}

char** get_args(const char* input, int* argc) {
    uint64_t num_args = 1;
    for (uint64_t i = 0; input[i] != '\0'; i++)
        num_args += (input[i] == ' ');

    char** args = sys_malloc((num_args + 1) * sizeof(char*));

    uint64_t arg_start = 0;
    uint64_t arg_count = 0;

    for (uint64_t i = 0; ; i++) {
        if (input[i] == ' ' || input[i] == '\0') {
            uint64_t arg_len = i - arg_start;
            char* arg = sys_malloc((arg_len + 1) * sizeof(char));

            for (uint64_t j = 0; j < arg_len; j++)
                arg[j] = input[arg_start + j];

            arg[arg_len] = '\0';

            args[arg_count++] = arg;

            if (input[i] == '\0')
                break;
            arg_start = i + 1;
        }
    }
    *argc = arg_count;
    args[arg_count] = NULL;
    return args;
}

char* get_last_arg(char** args) {
    for (uint64_t i = 0; args[i] != NULL; i++)
        if (args[i + 1] == NULL) return args[i];

    return NULL;
}

void free_args(char** args) {
    for (uint64_t i = 0; args[i] != NULL; i++)
        sys_free(args[i]);

    sys_free(args);
}

uint64_t nicent(char** argv, int argc) {
    
    if (argc < 3) {
        puts_with_color("nicent: ERROR must provide pid and priority\n", 0xFF0000);
        return -1;
    }
    
    uint64_t pid = atoi(argv[1]);
    uint64_t priority;


    if(strcmp(argv[2], "LOW") == 0) priority = 0;
    else if(strcmp(argv[2], "MEDIUM") == 0) priority = 1;
    else if(strcmp(argv[2], "HIGH") == 0) priority = 2;
    else {
        puts_with_color("nicent: ERROR priority must be LOW, MEDIUM or HIGH\n", 0xFF0000);
        return -1;
    }

    if (sys_nicent(pid, priority) == -1) {
        puts("nicent: ERROR changing priority\n");
        return -1;
    }

    return 0;
}

uint64_t kill(char** argv, int argc) {
    if (argc < 2) {
        puts_with_color("kill: ERROR must provide pid\n", 0xFF0000);
        return -1;
    }

    int pid = atoi(argv[1]);

    sys_kill_process_by_pid(pid);

    return 0;
}
