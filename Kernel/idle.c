#include <idle.h>
#include <drivers/videoDriver.h>
#include <interruptHandlers/interrupts.h>
#include <managers/processManager.h>
#include <shell_caller.h>

extern processManagerADT the_process_manager;

uint64_t idle(char** argv, int argc){
    set_font_size(1);
    clear_video_text_buffer();
    write_to_video_text_buffer("GRUPO 21\n", 9, 0x006fb5fb);
    write_to_video_text_buffer("Kernel initialized\nRunning user code...\n\n", 41, HEX_GRAY);
    create_process(the_process_manager, 0, 0, start_shell, argv);
    create_process(the_process_manager, 0, 0, idle2, argv);
    create_process(the_process_manager, 0, 0, idle3, argv);
    for(int i=0; i<5; i++) {
        write_to_video_text_buffer("hola\n", 5, 0x00ff0000);
        _hlt();
    }
    while(1);
}

uint64_t idle2(char** argv, int argc){
    for(int i=0; i<5; i++) {
        write_to_video_text_buffer("chau\n", 5, 0x0000ff00);
        _hlt();
    }
    while(1);
}

uint64_t idle3(char** argv, int argc){
    for(int i=0; i<5; i++) {
        write_to_video_text_buffer("chau\n", 5, 0x000000ff);
        _hlt();
    }
    while(1);
}
