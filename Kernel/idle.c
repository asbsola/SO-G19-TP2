#include <idle.h>
#include <drivers/videoDriver.h>
#include <interruptHandlers/interrupts.h>


uint64_t idle(char** argv, int argc){
    while(1) {
        write_to_video_text_buffer("hola\n", 5, 0x00ff0000);
        _hlt();
    }
}

uint64_t idle2(char** argv, int argc){
    while(1) {
        write_to_video_text_buffer("chau\n", 5, 0x0000ff00);
        _hlt();
    }
}

uint64_t idle3(char** argv, int argc){
    while(1) {
        write_to_video_text_buffer("chau\n", 5, 0x000000ff);
        _hlt();
    }
}
