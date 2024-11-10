// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <programs/programs.h>

uint64_t info(char **argv, int argc) {
	puts("screen info:\n");
	printf("width: %d pixels\n", sys_get_screen_width());
	printf("height: %d pixels\n\n", sys_get_screen_height());

	char cpu_vendor_buff[200];
	sys_get_cpu_vendor(cpu_vendor_buff);
	printf("cpu vendor: %s\n\n", cpu_vendor_buff);

	return 0;
}

uint64_t time(char **argv, int argc) {
	char *time = sys_get_time(-3);
	printf("%s\n", time);

	return 0;
}

uint64_t regs(char **argv, int argc) {
	sys_print_registers();

	return 0;
}

uint64_t calculator(char **argv, int argc) {
	uint32_t num1 = 0;
	uint32_t num2 = 0;
	uint32_t response = 0;

	char operator;

	printf("Enter only positive numbers.\n");

	printf("First number: ");
	scan_line("%d", &num1);

	printf("Operator: ");
	scan_line("%c", &operator);

	printf("Second number: ");
	scan_line("%d", &num2);

	switch (operator) {
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

uint64_t jump(char **argv, int argc) {
	uint64_t dir = 0;
	printf("Address: ");
	scan_line("%d", &dir);
	printf("Jumping to address %d\n", dir);
	jump_to_dir(dir);

	return 0;
}

uint64_t mem(char **argv, int argc) {
	printf("Total memory:    %d\n", sys_get_total_memory_size());
	printf("Usable memory:   %d\n", sys_get_usable_memory_size());
	printf("Free memory:     %d\n", sys_get_free_memory_size());
	printf("Occupied memory: %d\n",
			sys_get_usable_memory_size() - sys_get_free_memory_size());

	return 0;
}

const char *process_priority_names[] = {"LOW", "MEDIUM", "HIGH"};
const char *process_status_names[] = {"RUNNING", "READY", "BLOCKED", "EXITED",
	"KILLED"};

uint64_t ps(char **argv, int argc) {
	process_info_t *processes = (process_info_t *)sys_ps();
	for (int i = 0; processes[i].pid != -1; i++) {
		printf("\nProcess %d\n", i);
		printf("\tPID: %d\n", processes[i].pid);
		printf("\tName: %s\n", processes[i].name);

		if (processes[i].parent_pid == -1)
			printf("\tParent PID: -1\n");
		else
			printf("\tParent PID: %d\n", processes[i].parent_pid);

		printf("\tPriority: %s\n", process_priority_names[processes[i].priority]);
		printf("\tStatus: %s\n", process_status_names[processes[i].status]);

		if (sys_get_ppid() == processes[i].parent_pid)
			printf("\tForeground: %s IN FOREGROUND\n",
					(processes[i].parent_is_waiting == WAITING) ? "" : "NOT");
		printf("\tStack pointer: %d\n", processes[i].stack_pointer);
		printf("\tBase pointer: %d\n", processes[i].base_pointer);
	}

	sys_free(processes);

	return 0;
}

uint64_t loop(char **argv, int argc) {
	if (argc < 2) {
		puts_with_color("loop: ERROR must provide waiting time in seconds\n",
				0xFF0000);
		return -1;
	}

	pid_t pid = sys_get_pid();
	int time = atoi(argv[1]);

	while (1) {
		sleep(time * 1000);
		printf("My pid is %d\n", pid);
	}

	return 0;
}

#define BUF_SIZE 1024

uint64_t cat(char **argv, int argc) {
	char buffer[BUF_SIZE] = {0};

	fd_t stdin = sys_get_stdin();
	fd_t stdout = sys_get_stdout();
	int i = 1;
	char c;
	while (i > 0) {
		i = 0;
		c = 0;
		while (c != '\n' && sys_read(stdin, &c, 1) != EOF && i < BUF_SIZE - 1)
			buffer[i++] = c;
		buffer[i] = 0;
		sys_write(stdout, buffer, i + 1);
	}
	return 0;
}

uint64_t wc(char **argv, int argc) {
	char buffer[BUF_SIZE] = {0};

	fd_t stdin = sys_get_stdin();

	uint64_t read = 0;
	uint64_t lines = 0;

	while ((read = sys_read(stdin, buffer, BUF_SIZE)) != EOF) {
		for (int i = 0; i < read; i++) {
			if (buffer[i] == '\n')
				lines++;
		}
	}
	printf("Lines: %d\n", lines);
	return 0;
}

uint64_t filter(char **argv, int argc) {
	char filtered_buffer[BUF_SIZE] = {0};

	fd_t stdin = sys_get_stdin();
	fd_t stdout = sys_get_stdout();
	int i = 0;
	char c = 0;
	while (sys_read(stdin, &c, 1) != EOF && i < BUF_SIZE - 1) {
		if (!isVowel(c))
			filtered_buffer[i++] = c;
	}
	filtered_buffer[i] = 0;
	sys_write(stdout, filtered_buffer, i + 1);
	return 0;
}

uint64_t echo(char **argv, int argc) {
	for (int i = 1; i < argc; i++) {
		printf("%s ", argv[i]);
	}
	putchar('\n');

	return 0;
}
