#ifndef __PHYLO__
#define __PHYLO__

#include <syscall_adapters.h>
#include <test_utils.h>
#include <std.h>
#include <def.h>

#define MAX_NUMBER_OF_PHILOSOPHERS 15 
#define INITIAL_NUMBER_OF_PHILOSOPHERS 5
#define VIEW_PIPE "view_philosophers_pipe"
#define ADD_REMOVE_MUTEX "add_remove_mutex"

typedef enum {EATING, THINKING} philosopherState;
typedef struct {
    philosopherState state;
    pid_t pid;
} philosopher;

uint64_t phylo(char **argv, int argc);

#endif