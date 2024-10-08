#include <idle.h>
#include <drivers/videoDriver.h>
#include <interruptHandlers/interrupts.h>

uint64_t idle(char** argv, int argc){
    while(1) {
        write_to_video_text_buffer("hola\n", 5, 0x00ffffff);
        _hlt();
    }
}
