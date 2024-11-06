// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <std.h>
#include <stddef.h>
#include <test_utils.h>

#define TOTAL_PAIR_PROCESSES (TOTAL_PROCESSES/2)
#define MAX_LEN 250
#define PIPE_NAME "test_pipe"

uint64_t writer(char **argv, int argc) {
    char buff[MAX_LEN];
    int len = atoi(argv[2]);
    for(int i=0; i<len; i++){
        buff[i] = argv[1][i] - '0';
    }
    sys_write(sys_get_stdout(), buff, len);
    return 0;
}

uint64_t reader(char **argv, int argc) {
    char buff[MAX_LEN];
    int len = atoi((argv[1]));
    sys_read(sys_get_stdin(), buff, len);
    int res = 0;
    for(int i=0; i<len; i++) res += buff[i];
    return res;
}

uint64_t test_pipes(char **argv, int argc) {
    pid_t r_pids[TOTAL_PAIR_PROCESSES];
    int max_pair_processes;
    int msg_len;
    char buff[MAX_LEN] = {0};
    char len_w[4] = {0};
    char len_r[4] = {0};
    char* writerArgv[] = {"writer", buff, len_w, NULL};
    char* readerArgv[] = {"reader", len_r, NULL};
    int sent = 0;
    int64_t res = 0;

    if (argc < 3) {
        puts_with_color("test_pipes: ERROR must provide msg_len and max_pair_processes (tops at 125)\n", 0xFF0000);
        return -1;
    }

    msg_len = atoi(argv[1]);
    if (msg_len <= 0) {
        puts_with_color("test_pipes: ERROR msg_len must be greater than 0\n", 0xFF0000);
        return -1;
    }
    if(msg_len > MAX_LEN){
        puts_with_color("test_pipes: ERROR msg_len must be less than 250\n", 0xFF0000);
        return -1;
    }

    max_pair_processes = atoi(argv[2]);
    if (max_pair_processes <= 0) {
        puts_with_color("test_pipes: ERROR max_pair_processes must be greater than 0\n", 0xFF0000);
        return -1;
    }
    if(max_pair_processes > TOTAL_PAIR_PROCESSES){
        puts_with_color("test_pipes: ERROR max_pair_processes must be less than 250\n", 0xFF0000);
        return -1;
    }

    fd_t pipe = sys_pipe_open_named(PIPE_NAME);

    for(int i=0; i<max_pair_processes; i++){
        for(int j=0; j<msg_len; j++){
            int val = GetUniform(10);
            sent += val;
            writerArgv[1][j] = val + '0';
        }
        itoa(msg_len, writerArgv[2], 3);
        itoa(msg_len, readerArgv[1], 3);
        pid_t w_pid = sys_create_process(writer, writerArgv, sys_get_stdin(), pipe);
        r_pids[i] = sys_create_process(reader, readerArgv, pipe, sys_get_stdout());
        if(w_pid == -1 || r_pids[i] == -1){
            puts_with_color("test_pipe: ERROR creating process\n", 0xFF0000);
            sys_pipe_close(pipe);
            return -1;
        }
    }

    for(int i=0; i<max_pair_processes; i++){
        int64_t ret;
        sys_wait_pid(r_pids[i], &ret);
        res += ret;
    }

    sys_pipe_close(pipe);

    if(sent > res){
        puts_with_color("Missing sent values\n", 0x00ff0000);
        printf("Expected %d but received %d\n", sent, res);
        return -1;
    }
    if(sent < res){
        puts_with_color("Received extranious values\n", 0x00ff0000);
        printf("Expected %d but received %d\n", sent, res);
        return -1;
    }
    
    puts_with_color("All data recovered\n", 0x0000ff00);
    return 0;
}