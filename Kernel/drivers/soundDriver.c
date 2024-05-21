#include <stdint.h>
#include <timer.h>
#include <drivers/soundDriver.h>
#include <lib.h>

void beep(uint64_t freq, uint64_t secs) {
    play_sound(freq);
    int start = ticks_elapsed();
    while(ticks_elapsed() - start < secs*TICKS_PER_SECOND);
    stop_sound();
}