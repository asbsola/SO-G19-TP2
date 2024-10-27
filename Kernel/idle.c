// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <idle.h>
#include <drivers/videoDriver.h>
#include <interruptHandlers/interrupts.h>
#include <managers/processManager.h>
#include <shell_caller.h>

extern processManagerADT the_process_manager;
extern void yield();

void remove_orphans(processManagerADT process_manager){
    processControlBlockADT* processes = get_processes(process_manager);
    for(int i=get_max_pid(process_manager); i>=0; i--){
        if(processes[i] != NULL && processes[i]->parent_pid == IDLE_PROCESS_PID && processes[i]->status == EXITED){
            remove_process(process_manager, processes[i]->pid);
        }
    }
}

uint64_t idle(char** argv, int argc){
    set_font_size(1);
    clear_video_text_buffer();

    write_to_video_text_buffer("GRUPO 19\n", 9, 0x006fb5fb);

    char* shell_args[] = {"shell", NULL};
    create_process(the_process_manager, 0, (uint64_t (*)(char**, int))(SHELL_CODE_ADDRESS), shell_args);
    
    while(1) {
        remove_orphans(the_process_manager);
        yield();
    }
}
