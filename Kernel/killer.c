#include <killer.h>

extern processManagerADT the_process_manager;

uint64_t killer(char** argv, int argc){
    if(the_process_manager == NULL) return -1;
    return kill_signal(the_process_manager);
}