#include <idle.h>
#include <drivers/videoDriver.h>
#include <interruptHandlers/interrupts.h>

uint64_t idle(char** argv, int argc){
    while(1){
        write_to_video_text_buffer("helooooo\n", 18, HEX_GRAY);
        _hlt();
    }
}
