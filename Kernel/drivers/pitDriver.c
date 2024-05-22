#include <drivers/pitDriver.h>
#include <drivers/videoDriver.h>

static unsigned long ticks = 0;

void timer_handler(const registers64_t * registers) {
	ticks++;
    update_frame_buffer();
}

void delay(uint64_t milis){
	uint64_t start = ticks_elapsed();
	while(ticks_elapsed() - start < (TICKS_PER_SECOND * milis) / 1000);
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}
