#ifndef _rtc_H_
#define _rtc_H_

#include <stdint.h>

char *get_time(int time_zone);

uint8_t get_hours();
uint8_t get_minutes();
uint8_t get_seconds();

#endif
