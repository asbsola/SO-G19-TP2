#ifndef __TESTS__H__
#define __TESTS__H__

#include <stdint.h>
#include <std.h>
#include <stddef.h>
#include <syscall_adapters.h>
#include <test_utils.h>
#include <lib.h>

uint64_t test_mm(char** argv, int argc);
uint64_t test_processes(char** argv, int argc);
uint64_t test_wait(char** argv, int argc);
uint64_t test_prio(char** argv, int argc);
uint64_t test_prio_dist(char** argv, int argc);
uint64_t test_idle_cleanup(char** argv, int argc);
uint64_t test_sync(char** argv, int argc);
uint64_t test_pipes(char** argv, int argc);

#endif