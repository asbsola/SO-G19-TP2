#include <killer.h>

extern processManagerADT the_process_manager;

uint64_t killer(char** argv, uint64_t argc){
    return kill_signal(the_process_manager);
}