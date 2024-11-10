#ifndef __PIT__H__
#define __PIT__H__

#include <managers/scheduler.h>
#include <registers.h>

#define UPDATE_SCREEN_FREQ 30
#define MAX_FREQUENCY 1193182

void initialize_pit(uint32_t frequency);
uint64_t timer_handler(schedulerADT scheduler, const registers64_t *);
void delay(uint64_t milis);
int ticks_elapsed();

#endif
