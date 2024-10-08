#include <idle.h>
#include <drivers/videoDriver.h>
#include <interruptHandlers/interrupts.h>
#include <managers/processManager.h>
#include <shell.h>

extern processManagerADT the_process_manager;

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

    write_to_video_text_buffer("GRUPO 21\n", 9, 0x006fb5fb);

    create_process(the_process_manager, 0, NOT_IN_FOREGROUND, (uint64_t (*)(char**, int))(SHELL_CODE_ADDRESS), argv);
    
    while(1) {
        remove_orphans(the_process_manager);
        _hlt();
    }
}
