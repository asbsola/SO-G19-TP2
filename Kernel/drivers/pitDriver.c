#include <interruptHandlers/interrupts.h>
#include <drivers/pitDriver.h>
#include <drivers/videoDriver.h>
#include <lib.h>

static uint32_t TICKS_PER_SECOND;
static unsigned long ticks = 0;

static uint64_t UPDATE_SCREEN_RATE;
static unsigned long ticks_at_last_update = 0;

void initialize_pit(uint32_t frequency){
	TICKS_PER_SECOND = frequency;
    UPDATE_SCREEN_RATE = (TICKS_PER_SECOND >= UPDATE_SCREEN_FREQ) ? (TICKS_PER_SECOND / UPDATE_SCREEN_FREQ) : TICKS_PER_SECOND;
    uint16_t divisor = MAX_FREQUENCY / TICKS_PER_SECOND;
    outb(0x43, 0x36);
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}

uint64_t timer_handler(schedulerADT scheduler, const registers64_t * registers) {
	ticks++;
    if (ticks - ticks_at_last_update >= UPDATE_SCREEN_RATE) {
        update_frame_buffer();
        ticks_at_last_update = ticks;
    }

    return context_switch(scheduler, (uint64_t)registers);
}

void delay(uint64_t milis){
	uint64_t start = ticks_elapsed();
	while(ticks - start < (TICKS_PER_SECOND * milis) / 1000);
}

void delay_ticks(uint64_t ticks){
	uint64_t start = ticks_elapsed();
	while(ticks - start < ticks);
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}
