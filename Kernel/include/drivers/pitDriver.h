#ifndef _pit_H_
#define _pit_H_

#include <registers.h>
#include <managers/scheduler.h>

#define UPDATE_SCREEN_FREQ 30 
#define MAX_FREQUENCY 1193182

void initialize_pit(uint32_t frequency);
uint64_t timer_handler(schedulerADT scheduler, const registers64_t*);
void delay(uint64_t milis);
void delay_ticks(uint64_t ticks);
int ticks_elapsed();
int seconds_elapsed();

#endif
