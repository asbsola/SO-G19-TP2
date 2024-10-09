#include <idle.h>
#include <drivers/videoDriver.h>
#include <interruptHandlers/interrupts.h>

uint64_t idle(char** argv, int argc){
    while(1) {
        write_to_video_text_buffer("holas\n", 6, 0x00ffffff);
        //_hlt();
    }
}
