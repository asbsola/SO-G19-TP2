#ifndef __MEMORY_MANAGER__H__
#define __MEMORY_MANAGER__H__

#include <def.h>
#include <stddef.h>
#include <stdint.h>

#define ALIGNMENT 8

typedef struct memoryManagerCDT *memoryManagerADT;

memoryManagerADT init_memory_manager(void *memory, uint64_t memory_size);
void *mem_alloc(memoryManagerADT mem_manager, uint64_t size);
void mem_free(memoryManagerADT mem_manager, void *address);

uint64_t get_usable_memory_size(memoryManagerADT mem_manager);
uint64_t get_free_memory_size(memoryManagerADT mem_manager);
uint64_t get_total_memory_size(memoryManagerADT mem_manager);

#endif
