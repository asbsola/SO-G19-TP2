#include <stdint.h>
#include <timer.h>
#include <drivers/soundDriver.h>
#include <lib.h>

void beep(uint64_t freq, uint64_t milis) {
    if(freq == 0) return;
    play_sound(freq);
    delay(milis);
    stop_sound();
}