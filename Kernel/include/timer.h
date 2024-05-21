#ifndef _TIME_H_
#define _TIME_H_

#include <registers.h>

#define TICKS_PER_SECOND 18

void timer_handler(const registers64_t*);
int ticks_elapsed();
int seconds_elapsed();

#endif
