#ifndef __MEMORY_MANAGER_H__
#define __MEMORY_MANAGER_H__

#include <stdint.h>
#include <stddef.h>

#define ALIGNMENT 8

typedef struct memoryManagerCDT *memoryManagerADT;

memoryManagerADT init_memory_manager(void *memory, uint64_t memory_size);
void *mem_alloc(memoryManagerADT mem_manager, uint64_t size);
void mem_free(memoryManagerADT mem_manager, void *address);

uint64_t get_usable_memory_size(memoryManagerADT mem_manager);
uint64_t get_free_memory_size(memoryManagerADT mem_manager);
uint64_t get_total_memory_size(memoryManagerADT mem_manager);

#endif
