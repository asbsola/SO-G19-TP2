#ifndef __SCREEN_SERVICE__H__
#define __SCREEN_SERVICE__H__

#include <def.h>
#include <drivers/videoDriver.h>
#include <utils/utils.h>

#define LINE_MAX_LEN 1024

uint64_t screen_service(char **argv, int argc);

#endif