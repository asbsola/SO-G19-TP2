#include <drivers/pitDriver.h>
#include <drivers/videoDriver.h>
#include <lib.h>

const uint64_t MAX_FREQUENCY = 1193182;
static uint32_t TICKS_PER_SECOND;
static unsigned long ticks = 0;

void initialize_pit(uint32_t frequency){
	TICKS_PER_SECOND = frequency;
    uint16_t divisor = MAX_FREQUENCY / TICKS_PER_SECOND;
    outb(0x43, 0x36);
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}

void timer_handler(const registers64_t * registers) {
	ticks++;
    update_frame_buffer();
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
