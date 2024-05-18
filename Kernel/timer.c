#include <timer.h>
#include <drivers/videoDriver.h>

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
    update_frame_buffer();
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}
