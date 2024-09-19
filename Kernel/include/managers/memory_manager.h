#ifndef __MEMORY_MANAGER_H__
#define __MEMORY_MANAGER_H__

#include <stdint.h>

typedef struct memoryManagerCDT* memoryManagerADT;

void init_memory_manager(memoryManagerADT mem_manager, void* memory, uint64_t memory_size);
void* malloc(memoryManagerADT mem_manager, uint64_t size);
void free(memoryManagerADT mem_manager, void* address);

#endif
