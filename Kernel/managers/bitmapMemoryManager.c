// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <managers/memoryManager.h>
#include <stdio.h>

#define BITS_PER_BYTE 8
#define BITS_PER_BLOCK 2
#define BLOCKS_PER_BYTE (BITS_PER_BYTE / BITS_PER_BLOCK)
#define BLOCK_SIZE 512
typedef uint8_t *memoryBitMap;

struct memoryManagerCDT
{
    memoryBitMap bitmap;
    void *memory_start;
    void *bitmap_start;
    uint64_t bitmap_length;
    uint64_t max_blocks;
    uint64_t usable_memory_size;
    uint64_t free_memory_size;
};

#define FREE 0
#define USED 1
#define START 3
#define GET_BLOCK_STATE(val, b) (((val) & (3 << ((b) * BITS_PER_BLOCK))) >> ((b) * BITS_PER_BLOCK))
#define SET_STATE_FREE(val, b) ((val) &= ~(3 << ((b) * BITS_PER_BLOCK)))
#define SET_STATE_USED(val, b) ((val) |= 1 << ((b) * BITS_PER_BLOCK))
#define SET_STATE_START(val, b) ((val) |= 3 << ((b) * BITS_PER_BLOCK))

memoryManagerADT init_memory_manager(void *memory, uint64_t memory_size)
{
    memoryManagerADT mem_manager = memory;
    mem_manager->bitmap = memory + sizeof(struct memoryManagerCDT);
    mem_manager->bitmap_start = mem_manager->bitmap;
    int blocks_per_entry = sizeof(mem_manager->bitmap[0]) * BLOCKS_PER_BYTE;
    uint64_t bitmap_max_length = (memory_size - sizeof(struct memoryManagerCDT)) / BLOCK_SIZE / blocks_per_entry;

    uint64_t non_usable_memory_size = sizeof(struct memoryManagerCDT) + bitmap_max_length * sizeof(mem_manager->bitmap[0]);
    if (memory_size < non_usable_memory_size)
        return NULL;

    mem_manager->max_blocks = (memory_size - non_usable_memory_size) / BLOCK_SIZE;
    if (mem_manager->max_blocks == 0)
        return NULL;

    mem_manager->bitmap_length = mem_manager->max_blocks / blocks_per_entry + (mem_manager->max_blocks % blocks_per_entry != 0);
    mem_manager->memory_start = mem_manager->bitmap_start + mem_manager->bitmap_length * sizeof(mem_manager->bitmap[0]);
    mem_manager->usable_memory_size = mem_manager->max_blocks * BLOCK_SIZE;
    mem_manager->free_memory_size = mem_manager->usable_memory_size;

    for (uint64_t i = 0; i < mem_manager->bitmap_length; i++)
    {
        mem_manager->bitmap[i] = 0;
    }
    return mem_manager;
}

uint64_t get_usable_memory_size(memoryManagerADT mem_manager)
{
    return mem_manager->usable_memory_size;
}

uint64_t get_free_memory_size(memoryManagerADT mem_manager)
{
    return mem_manager->free_memory_size;
}

uint64_t get_total_memory_size(memoryManagerADT mem_manager)
{
    return mem_manager->usable_memory_size + mem_manager->bitmap_length * sizeof(mem_manager->bitmap[0]) + sizeof(struct memoryManagerCDT);
}

int find_free_blocks(memoryManagerADT mem_manager, uint64_t blocks_needed, uint64_t *start_block)
{
    if (blocks_needed == 0)
        return -1;
    int blocks_per_entry = sizeof(mem_manager->bitmap[0]) * BLOCKS_PER_BYTE;
    uint64_t free_blocks = 0;
    for (uint64_t i = 0; i < mem_manager->bitmap_length; i++)
    {
        for (int b = 0; b < blocks_per_entry; b++)
        {
            if (i * blocks_per_entry + b >= mem_manager->max_blocks)
                break;
            if (GET_BLOCK_STATE(mem_manager->bitmap[i], b) != FREE)
                free_blocks = 0;
            else
            {
                free_blocks++;
                if (free_blocks >= blocks_needed)
                {
                    *start_block = i * blocks_per_entry + b - blocks_needed + 1;
                    return 1;
                }
            }
        }
    }
    return -1;
}

void reserve_blocks(memoryManagerADT mem_manager, uint64_t start_block, uint64_t blocks)
{
    int blocks_per_entry = sizeof(mem_manager->bitmap[0]) * BLOCKS_PER_BYTE;
    uint64_t index = start_block / blocks_per_entry;
    int block_offset = start_block % blocks_per_entry;
    SET_STATE_START(mem_manager->bitmap[index], block_offset);
    for (uint64_t b = 1; b < blocks; b++)
    {
        index = (start_block + b) / blocks_per_entry;
        block_offset = (start_block + b) % blocks_per_entry;
        SET_STATE_USED(mem_manager->bitmap[index], block_offset);
    }
    mem_manager->free_memory_size -= blocks * BLOCK_SIZE;
}

void *mem_alloc(memoryManagerADT mem_manager, uint64_t size)
{
    uint64_t blocks_needed = size / BLOCK_SIZE + (size % BLOCK_SIZE != 0);
    uint64_t start_block = 0;
    int res = find_free_blocks(mem_manager, blocks_needed, &start_block);
    if (res == -1)
        return NULL;
    reserve_blocks(mem_manager, start_block, blocks_needed);
    return mem_manager->memory_start + BLOCK_SIZE * start_block;
}

int valid_chunk_start(memoryManagerADT mem_manager, uint64_t offset)
{
    if (offset % BLOCK_SIZE != 0)
        return 0;
    uint64_t start_block = offset / BLOCK_SIZE;
    int blocks_per_entry = sizeof(mem_manager->bitmap[0]) * BLOCKS_PER_BYTE;
    uint64_t index = start_block / blocks_per_entry;
    if (index > mem_manager->bitmap_length)
        return 0;
    int block_offset = start_block % blocks_per_entry;
    return GET_BLOCK_STATE(mem_manager->bitmap[index], block_offset) == START;
}

void free_blocks(memoryManagerADT mem_manager, uint64_t start_block)
{
    int blocks_per_entry = sizeof(mem_manager->bitmap[0]) * BLOCKS_PER_BYTE;
    uint64_t index = start_block / blocks_per_entry;
    int block_offset = start_block % blocks_per_entry;
    for (uint64_t b = 0; b == 0 || GET_BLOCK_STATE(mem_manager->bitmap[index], block_offset) == USED; b++)
    {
        SET_STATE_FREE(mem_manager->bitmap[index], block_offset);
        index = (start_block + b + 1) / blocks_per_entry;
        block_offset = (start_block + b + 1) % blocks_per_entry;
        mem_manager->free_memory_size += BLOCK_SIZE;
    }
}

void mem_free(memoryManagerADT mem_manager, void *address)
{
    uint64_t offset = address - mem_manager->memory_start;
    if (!valid_chunk_start(mem_manager, offset))
        return;
    uint64_t start_block = offset / BLOCK_SIZE;
    free_blocks(mem_manager, start_block);
}
