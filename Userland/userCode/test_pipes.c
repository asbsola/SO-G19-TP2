// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <std.h>
#include <stddef.h>

#define TOTAL_PROCESSES 250
#define PIPE_NAME "test_pipe"

uint64_t writer(char **argv, int argc) {
    int n = atoi(argv[1]);
    char buff[] = {n};
    sys_write(sys_get_stdout(), buff, 1);
    return 0;
}

uint64_t reader(char **argv, int argc) {
    char buff[1];
    sys_read(sys_get_stdin(), buff, 1);
    return buff[0];
}

uint64_t test_pipes(char **argv, int argc) {
    pid_t pids[TOTAL_PROCESSES];
    int max_processes;
    char buff[3];
    char* writerArgv[] = {"writer", buff, NULL};
    char* readerArgv[] = {"reader", NULL};
    int sent = 0;
    int64_t res = 0;

    if (argc < 2) {
        puts_with_color("test_pipes: ERROR must provide max_processes (tops at 250)\n", 0xFF0000);
        return -1;
    }

    max_processes = atoi(argv[1]);
    if (max_processes <= 0) {
        puts_with_color("test_wait: ERROR max_processes must be greater than 0\n", 0xFF0000);
        return -1;
    }
    if(max_processes > TOTAL_PROCESSES){
        puts_with_color("test_wait: ERROR max_processes must be less than 250\n", 0xFF0000);
        return -1;
    }

    fd_t pipe = sys_pipe_open(PIPE_NAME);

    for(int i=1; i<=max_processes; i++){
        itoa(i, writerArgv[1], 3);
        sent += i;
        sys_create_process(writer, writerArgv, sys_get_stdin(), pipe);
    }

    for(int i=0; i<max_processes; i++)
        pids[i] = sys_create_process(reader, readerArgv, pipe, sys_get_stdout());

    for(int i=0; i<max_processes; i++){
        int64_t ret;
        sys_wait_pid(pids[i], &ret);
        res += ret;
    }

    sys_pipe_close_named(PIPE_NAME);

    if(sent < res){
        puts_with_color("Missing sent values\n", 0x00ff0000);
        return -1;
    }
    if(sent > res){
        puts_with_color("Received extranious values\n", 0x00ff0000);
        return -1;
    }
    
    puts_with_color("All data recovered\n", 0x0000ff00);
    return 0;
}