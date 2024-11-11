#ifndef __PHYLO__H__
#define __PHYLO__H__

#include <def.h>
#include <std.h>
#include <syscall_adapters.h>
#include <test_utils.h>

#define MAX_NUMBER_OF_PHILOSOPHERS 15
#define INITIAL_NUMBER_OF_PHILOSOPHERS 5
#define MIN_EAT_TIME 500
#define MIN_THINK_TIME 1000
#define RANDOM_EAT_RANGE 500
#define RANDOM_THINK_RANGE 1000
#define VIEW_PIPE "view_philosophers_pipe"
#define ADD_REMOVE_MUTEX "add_remove_mutex"

typedef enum { EATING, THINKING } philosopherState;
typedef struct {
	philosopherState state;
	pid_t pid;
} philosopher;

uint64_t phylo(char **argv, int argc);

#endif