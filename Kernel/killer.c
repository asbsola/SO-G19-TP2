#include <killer.h>

extern processManagerADT the_process_manager;

uint64_t killer(char** argv, int argc){
    if(the_process_manager == NULL) return -1;
    int recursive = (argc > 1 && argv[1][0] == 'r');
    return kill_signal(the_process_manager, recursive);
}