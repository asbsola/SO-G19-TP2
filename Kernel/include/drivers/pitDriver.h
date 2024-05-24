#ifndef _pit_H_
#define _pit_H_

#include <registers.h>

#define TICKS_PER_SECOND 18

void timer_handler(const registers64_t*);
void delay(uint64_t milis);
void delay_ticks(uint64_t ticks);
int ticks_elapsed();
int seconds_elapsed();

#endif
