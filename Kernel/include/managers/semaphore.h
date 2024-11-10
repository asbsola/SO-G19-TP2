#ifndef __SEMAPHORE__H__
#define __SEMAPHORE__H__

#include <def.h>
#include <utils/list.h>

struct semaphoreCDT {
  uint64_t value;
  uint8_t lock;
  char *name;
  ListADT waiting_processes;
};

typedef struct semaphoreCDT *semaphoreADT;

#endif
