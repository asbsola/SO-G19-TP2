#include <timer.h>
#include <drivers/videoDriver.h>

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
	//putPixel(0xFFFFFF, 100+ticks, 50);
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}
