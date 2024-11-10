// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <process.h>
#include <syscall_adapters.h>

#define RECURSIVE 1

void exit_self(int64_t status) {
  pid_t pid = sys_get_pid();
  sys_exit_process_by_pid(pid, status);
}

void kill_self() {
  pid_t pid = sys_get_pid();
  sys_kill_process_by_pid(pid, !RECURSIVE);
}

void block_self() {
  pid_t pid = sys_get_pid();
  sys_block_process_by_pid(pid);
}

void unblock_self() {
  pid_t pid = sys_get_pid();
  sys_unblock_process_by_pid(pid);
}
