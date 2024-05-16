#include <time.h>
#include <videoDriver.h>

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
	putPixel(0xFFFFFF, 100+ticks, 50);
	if(ticks%18 == 0)
	    for(int i=0; i<50; i++)
    	    for(int j=0; j<50; j++)
    		    putPixel(0x000000, 50+i, 100+j);
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}
