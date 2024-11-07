#ifndef __SCREEN_SERVICE_H__
#define __SCREEN_SERVICE_H__

#include <def.h>
#include <drivers/videoDriver.h>
#include <utils/utils.h>

#define LINE_MAX_LEN 1024

uint64_t screen_service(char** argv, int argc);

#endif