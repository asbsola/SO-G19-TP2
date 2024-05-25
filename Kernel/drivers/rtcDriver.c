#include <drivers/rtcDriver.h>
#include <stdint.h>

static char time[9];

char * get_time(int time_zone){
    uint8_t h = ((24 + get_hours() + time_zone) % 24);
    uint8_t m = get_minutes();
    uint8_t s = get_seconds();
    time[0] = '0' + (h >> 4);
    time[1] = '0' + (h & 0x0F);
    time[2] = ':';
    time[3] = '0' + (m >> 4);
    time[4] = '0' + (m & 0x0F);
    time[5] = ':';
    time[6] = '0' + (s >> 4);
    time[7] = '0' + (s & 0x0F);
    time[8] = 0;
    return time;
}
