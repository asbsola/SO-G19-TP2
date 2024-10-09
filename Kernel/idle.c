#include <idle.h>
#include <drivers/videoDriver.h>
#include <registers.h>
#include <managers/processControlBlock.h>
#include <interruptHandlers/interrupts.h>
#include <managers/processManager.h>
#include <shell.h>

extern processManagerADT the_process_manager;

extern void start_process_wrapper();

uint64_t idle(char** argv, int argc){
    set_font_size(1);
    clear_video_text_buffer();

    write_to_video_text_buffer("GRUPO 21\n", 9, 0x006fb5fb);

    create_process(the_process_manager, 0, NOT_IN_FOREGROUND, (uint64_t (*)(char**, int))(SHELL_CODE_ADDRESS), argv);
    
    while(1) _hlt();
}
