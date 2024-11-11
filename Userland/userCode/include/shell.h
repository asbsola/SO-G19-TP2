#ifndef __SHELL__H__
#define __SHELL__H__

#include <def.h>
#include <lib.h>
#include <programs/cucaracha.h>
#include <programs/eliminator.h>
#include <programs/phylo.h>
#include <programs/programs.h>
#include <std.h>
#include <stddef.h>
#include <syscall_adapters.h>
#include <tests.h>

#define MAX_SHELL_INPUT 1024
#define MAX_COMMANDS 10
typedef enum { BUILT_IN, PROCESS } command_type;

typedef struct {
	char *module_name;
	char *module_description;
	command_type module_type;
	uint64_t (*module)(char **, int);
} ModuleDescriptor;

typedef struct {
	ModuleDescriptor module;
	char **argv;
	int argc;
} Command;

uint64_t help(char **argv, int argc);
uint64_t cls(char **argv, int argc);
uint64_t font_size(char **argv, int argc);
uint64_t beep(char **argv, int argc);
uint64_t nicent(char **argv, int argc);
uint64_t kill(char **argv, int argc);
uint64_t cleanup(char **argv, int argc);
uint64_t block(char **argv, int argc);
uint64_t unblock(char **argv, int argc);

void run_shell();
uint64_t run_cmd(Command cmd, fd_t stdin, fd_t stdout);
void free_args(char **args);
int get_commands(char *shell_input, Command *commands, int *num_cmds);
int get_module_index(char *module_name);

#endif
