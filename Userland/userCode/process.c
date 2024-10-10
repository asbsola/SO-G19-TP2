#include <syscall_adapters.h>
#include <process.h>

void exit_self(int64_t status) {
    pid_t pid = sys_get_pid();
    sys_exit_process_by_pid(pid, status);
}

void kill_self() {
    pid_t pid = sys_get_pid();
    sys_kill_process_by_pid(pid);
}

void block_self() {
    pid_t pid = sys_get_pid();
    sys_block_process_by_pid(pid);
}

void unblock_self() {
    pid_t pid = sys_get_pid();
    sys_unblock_process_by_pid(pid);
}
