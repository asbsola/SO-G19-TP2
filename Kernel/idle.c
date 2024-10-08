#include <idle.h>
#include <drivers/videoDriver.h>
#include <interruptHandlers/interrupts.h>

uint64_t idle(char** argv, int argc){
    while(1){
        update_frame_buffer();
        _hlt();
    }
}