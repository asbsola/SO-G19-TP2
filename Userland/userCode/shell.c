// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <shell.h>
#include <def.h>
#include <lib.h>

ModuleDescriptor modules[] = {
    {"help", "displays available modules", BUILT_IN, help},
    {"clear", "clears the screens text buffer", BUILT_IN, cls},
    {"sysinfo", "displays system information", PROCESS, info},
    {"fontsize", "change font size", BUILT_IN, font_size},
    {"time", "display current time", PROCESS, time},
    {"regs", "displays captured registers (ESC key to capture)", PROCESS, regs},
    {"beep", "beeps", BUILT_IN, beep},
    {"song", "plays a short tune while displaying graphics", PROCESS, song},
    {"calculator", "positive integer calculator", PROCESS, calculator},
    {"eliminator", "eliminator game", PROCESS, eliminator},
    {"jump", "jumps to address given by user in decimal (12345 causes invalid opcode)", PROCESS, jump},
    {"test_mm", "tests the memory manager", PROCESS, test_mm},
    {"test_processes", "tests processes", PROCESS, test_processes},
    {"test_wait", "tests waits and return values", PROCESS, test_wait},
    {"test_priority", "tests priority", PROCESS, test_prio},
    {"test_priority_dist", "tests priority distribution", PROCESS, test_prio_dist},
    {"test_idle_cleanup", "tests idle cleanup of orphans", PROCESS, test_idle_cleanup},
    {"test_sync", "tests syncro", PROCESS, test_sync},
    {"test_pipes", "tests pipes", PROCESS, test_pipes},
    {"mega_test_pipes", "mega tests pipes", PROCESS, mega_test_pipes},
    {"mem", "displays memory status", PROCESS, mem},
    {"ps", "displays information about processes", PROCESS, ps},
    {"nicent", "changes process priority by PID", BUILT_IN, nicent},
    {"kill", "terminates a process by its PID. If the flag 'r' is provided, it also terminates all its descendants", BUILT_IN, kill},
    {"cleanup", "removes all exited processes", BUILT_IN, cleanup},
    {"block", "blocks a procces by a PID", BUILT_IN, block},
    {"unblock", "unblocks a procces by a PID", BUILT_IN, unblock},
    {"loop", "prints its PID every n seconds", PROCESS, loop},
    {"phylo", "the dining philosophers problem", PROCESS, phylo},
    {"cat", "print on the standard output", PROCESS, cat},
    {"wc", "counts the number of lines of input", PROCESS, wc},
    {"filter", "Filters the input vowels", PROCESS, filter},
    {"echo", "display a line of text", PROCESS, echo},
};

static int current_font_size = 1;

void run_shell()
{
    char shell_input[MAX_SHELL_INPUT];
    shell_input[0] = 0;
    Command commands[MAX_COMMANDS];
    pid_t pids[MAX_COMMANDS];
    fd_t pipes[MAX_COMMANDS - 1];
    int num_cmds = 0;
    sys_nicent(sys_get_pid(), HIGH);

    while (strcmp(shell_input, "exit") != 0)
    {
        sys_set_font_size(current_font_size);

        puts_with_color("shell> ", 0x006fb5fb);
        scanf("%s", shell_input);

        int64_t ans = get_commands(shell_input, commands, &num_cmds);
        if(ans == -1) continue;

        if (num_cmds == 1) {
            pids[0] = run_cmd(commands[0], sys_get_stdin(), sys_get_stdout());
        } else{
            int i;
            for (i = 0; i < num_cmds - 1; i++){
                pipes[i] = sys_pipe_open(NON_CANNONICAL);

                if (i == 0) {
                    pids[i] = run_cmd(commands[i], sys_get_stdin(), pipes[i]);
                } else {
                    pids[i] = run_cmd(commands[i], pipes[i - 1], pipes[i]);
                }

            }
            pids[i] = run_cmd(commands[num_cmds - 1], pipes[num_cmds - 2], sys_get_stdout());
        }
        for(int i = 0; i < num_cmds; i++){
            if (commands[i].argv[commands[i].argc - 1][0] != '&') {
                sys_wait_pid(pids[i], &ans); 
            }
            if (i < num_cmds - 1) {
                sys_pipe_send_eof(pipes[i]);
            }
        }

        for (int i = 0; i < num_cmds - 1; i++){
            sys_pipe_close(pipes[i]);
            free_args(commands[i].argv); 
        }
        free_args(commands[num_cmds - 1].argv); 
    }
}

int get_module_index(char* module_name){
    for (uint32_t i = 0; i < sizeof(modules) / sizeof(modules[0]); i++) {
        if (strcmp(module_name, modules[i].module_name) == 0) {
            return i;
        }
    }
    return -1;
}

void free_commands_argv(Command* commands, int num_cmds) {
    for (int i = 0; i < num_cmds; i++)
        free_args(commands[i].argv);
}

int get_commands(char* shell_input, Command* commands, int* num_cmds){
    char ** cmds = split(shell_input, num_cmds, '|');
    for (int i = 0; i < *num_cmds; i++){
        trim(cmds[i]);
    }
    for (int i = 0; i < *num_cmds; i++){
        commands[i].argv = split(cmds[i], &commands[i].argc, ' ');

        for (int j = 0; j < commands[i].argc; j++){
            trim(commands[i].argv[j]);
        }

        int module_index = get_module_index(commands[i].argv[0]);
        if (module_index == -1){
            printf("Unknown command: %s\n", commands[i].argv[0]);
            free_args(cmds);
            free_commands_argv(commands, i + 1);
            return -1;
        }
        if (modules[module_index].module_type == BUILT_IN && *num_cmds > 1){
            printf("Built-in command %s cannot be piped\n", commands[i].argv[0]);
            free_args(cmds);
            free_commands_argv(commands, i + 1);
            return -1;
        }
        commands[i].module = modules[module_index];
    }

    free_args(cmds);
    return 0;
}


uint64_t run_cmd(Command cmd, fd_t stdin, fd_t stdout)
{
    pid_t pid = -1;

    if (cmd.module.module_type == PROCESS) {
        pid = sys_create_process(cmd.module.module, cmd.argv, stdin, stdout);
        if(pid == -1) {
            puts_with_color("Error creating process\n", 0xFF0000);
            return -1;
        }
    }
    else if (cmd.module.module_type == BUILT_IN) 
        cmd.module.module(cmd.argv, cmd.argc);
    

    return pid;
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
        if(num2 == 0) {
            printf("Division by zero is not allowed.\n");
            return -1;
        }
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

        if(sys_get_ppid() == processes[i].parent_pid)
            printf("\tForeground: %s IN FOREGROUND\n", (processes[i].parent_is_waiting == WAITING) ? "" : "NOT");
        printf("\tStack pointer: %d\n", processes[i].stack_pointer);
        printf("\tBase pointer: %d\n", processes[i].base_pointer);
    }

    sys_free(processes); 

    return 0;
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

    uint64_t kill_recursive = 0;

    if (argv[2][0] == 'r') {
        kill_recursive = 1;
    }

    int pid = atoi(argv[1]);


    if (sys_kill_process_by_pid(pid, kill_recursive) == -1){
        puts_with_color("kill: ERROR could not kill process\n", 0xFF0000);
        return -1;
    }

    return 0;
}

uint64_t cleanup(char** argv, int argc) {
    int64_t ret;
    while(sys_wait(&ret) != -1);
    return 0;
}

uint64_t block(char** argv, int argc) {
    if (argc < 2) {
        puts_with_color("block: ERROR must provide pid\n", 0xFF0000);
        return -1;
    }
    int pid = atoi(argv[1]);
    
    return sys_block_process_by_pid(pid);
}

uint64_t unblock(char** argv, int argc) {
    if (argc < 2) {
        puts_with_color("unblock: ERROR must provide pid\n", 0xFF0000);
        return -1;
    }
    int pid = atoi(argv[1]);
    
    return sys_unblock_process_by_pid(pid);
}

uint64_t loop(char** argv, int argc) {
    if (argc < 2) {
        puts_with_color("loop: ERROR must provide waiting time in seconds\n", 0xFF0000);
        return -1;
    }

    pid_t pid = sys_get_pid();
    int time = atoi(argv[1]);
    
    while(1) {
        sleep(time * 1000);
        printf("My pid is %d\n", pid);
    }

    return 0;
}


uint64_t cat(char** argv, int argc) {
    const int max_len = 1024;
    char buffer[max_len];
    
    fd_t stdin = sys_get_stdin();
    fd_t stdout = sys_get_stdout();
    int i = 1;
    while (i > 0){
        i = 0;
        char c = 0;
        while (c != '\n' && sys_read(stdin, &c, 1) != EOF && i < max_len-1) buffer[i++] = c;
        buffer[i] = 0;
        sys_write(stdout, buffer, i+1);
    }
    return 0;
}

uint64_t wc(char** argv, int argc) {
    const int max_len = 1024;
    char buffer[max_len];
    
    fd_t stdin = sys_get_stdin();

    uint64_t read = 0;
    uint64_t lines = 0;

    while ((read = sys_read(stdin, buffer, max_len)) != EOF) {
        for (int i = 0; i < read; i++) {
            if(buffer[i] == '\n') lines++;
        }
    }
    printf("Lines: %d\n", lines);
    return 0;
}

uint64_t filter(char** argv, int argc) {
    const int max_len = 1024;
    char filtered_buffer[max_len];
    
    fd_t stdin = sys_get_stdin();
    fd_t stdout = sys_get_stdout();
    int i = 0;
    char c = 0;
    while (sys_read(stdin, &c, 1) != EOF && i < max_len-1) {
        if (!isVowel(c))
            filtered_buffer[i++] = c;
    }
    filtered_buffer[i] = 0;
    sys_write(stdout, filtered_buffer, i+1);
    return 0;
}

uint64_t echo(char** argv, int argc) {
    if (argc >= 2) {
        puts(argv[1]);
    }
    putchar('\n');

    return 0;
}
