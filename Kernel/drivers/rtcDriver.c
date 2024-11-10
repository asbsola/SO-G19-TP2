// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <drivers/rtcDriver.h>
#include <stdint.h>

static char time[9];

char *get_time(int time_zone) {
	uint8_t h = get_hours();
	h = (h >> 4) * 10 + (h & 0x0F);
	h = (24 + h + time_zone) % 24;
	uint8_t m = get_minutes();
	uint8_t s = get_seconds();
	time[0] = '0' + (h / 10);
	time[1] = '0' + (h % 10);
	time[2] = ':';
	time[3] = '0' + (m >> 4);
	time[4] = '0' + (m & 0x0F);
	time[5] = ':';
	time[6] = '0' + (s >> 4);
	time[7] = '0' + (s & 0x0F);
	time[8] = 0;
	return time;
}
