#ifndef __RTC__H__
#define __RTC__H__

#include <stdint.h>

char *get_time(int time_zone);

uint8_t get_hours();
uint8_t get_minutes();
uint8_t get_seconds();

#endif
