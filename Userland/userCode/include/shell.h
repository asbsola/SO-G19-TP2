#ifndef __SHELL__
#define __SHELL__

#include <syscall_adapters.h>
#include <std.h>
#include <cucaracha.h>
#include <eliminator.h>
#include <lib.h>
#include <stddef.h>

#define MAX_SHELL_INPUT 1024

typedef enum {BUILT_IN, PROCESS} command_type;

typedef struct
{
    char *module_name;
    char *module_description;
    command_type module_type;
    uint64_t (*module)(char**, int);
} ModuleDescriptor;

uint64_t help(char** argv, int argc);
uint64_t cls(char** argv, int argc);
uint64_t info(char** argv, int argc);
uint64_t font_size(char** argv, int argc);
uint64_t time(char** argv, int argc);
uint64_t regs(char** argv, int argc);
uint64_t beep(char** argv, int argc);
uint64_t song(char** argv, int argc);
uint64_t calculator(char** argv, int argc);
uint64_t eliminator(char** argv, int argc);
uint64_t jump(char** argv, int argc);
uint64_t test_mm(char** argv, int argc);
uint64_t test_processes(char** argv, int argc);
uint64_t test_wait(char** argv, int argc);
uint64_t test_prio(char** argv, int argc);
uint64_t test_prio_dist(char** argv, int argc);
uint64_t test_idle_cleanup(char** argv, int argc);
uint64_t test_sync(char** argv, int argc);
uint64_t test_pipes(char** argv, int argc);
uint64_t mega_test_pipes(char **argv, int argc);
uint64_t mem(char** argv, int argc);
uint64_t ps(char** argv, int argc);
uint64_t nicent(char** argv, int argc);
uint64_t kill(char** argv, int argc);
uint64_t cleanup(char** argv, int argc);
uint64_t block(char** argv, int argc);
uint64_t unblock(char** argv, int argc);
uint64_t loop(char** argv, int argc);
uint64_t phylo(char** argv, int argc);
uint64_t cat(char** argv, int argc);
uint64_t wc(char** argv, int argc);
uint64_t filter(char** argv, int argc);

void run_shell();
char** get_args(const char* input, int* argc);
void free_args(char** args);

#endif
