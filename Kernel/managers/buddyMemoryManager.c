// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <managers/memoryManager.h>
#include <stdio.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define MIN_BLOCK_SIZE_ORDER 8 // min_block_size = 2^order
// #define NULL 0

uint64_t nlog2(uint64_t n);
uint64_t _2pown(uint64_t n);

typedef enum
{
    FREE,
    SPLIT,
    USED
} memoryNodeStatus;

typedef struct memoryNode
{
    memoryNodeStatus status;
} memoryNode;

struct memoryManagerCDT
{
    memoryNode *root;
    void *memory_start;
    void *tree_memory_start;
    uint64_t tree_max_height;
    uint64_t memory_size;
    uint64_t free_memory_size;
};

memoryManagerADT init_memory_manager(void *memory, uint64_t memory_size)
{
    uint64_t max_block_order = nlog2(memory_size);

    if (max_block_order < MIN_BLOCK_SIZE_ORDER)
        return NULL;

    uint64_t tree_max_height = max_block_order - MIN_BLOCK_SIZE_ORDER + 1;
    uint64_t tree_max_node_count = _2pown(tree_max_height) - 1;
    uint64_t tree_max_size = sizeof(memoryNode) * tree_max_node_count;

    if (memory_size < tree_max_size)
        return NULL;

    uint64_t usable_memory = _2pown(nlog2(memory_size - tree_max_size));

    uint64_t min_block_size = _2pown(MIN_BLOCK_SIZE_ORDER);
    if (usable_memory < min_block_size)
        return NULL;

    memoryManagerADT mem_manager = memory;
    mem_manager->tree_memory_start = memory + sizeof(struct memoryManagerCDT);
    mem_manager->memory_start = (void *)(((uint64_t)(mem_manager->tree_memory_start + tree_max_size) / ALIGNMENT + 1) * ALIGNMENT);
    mem_manager->memory_size = usable_memory;
    mem_manager->free_memory_size = usable_memory;
    mem_manager->tree_max_height = tree_max_height;

    memoryNode *root = memory + sizeof(struct memoryManagerCDT);

    for (uint64_t i = 0; i < tree_max_node_count; i++)
        root[i].status = FREE;

    mem_manager->root = root;

    return mem_manager;
}

uint64_t get_usable_memory_size(memoryManagerADT mem_manager)
{
    return mem_manager->memory_size;
}

uint64_t get_free_memory_size(memoryManagerADT mem_manager)
{
    return mem_manager->free_memory_size;
}

uint64_t get_total_memory_size(memoryManagerADT mem_manager)
{
    return (uint64_t)((uint64_t)mem_manager->memory_start - (uint64_t)mem_manager) + mem_manager->memory_size;
}

void *allocate_mem_recursive(memoryNode *tree, uint64_t size, uint64_t chunk_size, void *chunk_start, uint64_t node_index, uint64_t height)
{
    if (chunk_size < size || tree[node_index].status == USED)
        return NULL;

    if (chunk_size == size)
    {
        if (tree[node_index].status == SPLIT)
            return NULL;

        tree[node_index].status = USED;
        return chunk_start;
    }

    if (tree[node_index].status != SPLIT)
    {
        tree[node_index].status = SPLIT;
    }

    uint64_t half_size = chunk_size / 2;
    uint64_t left_index = node_index + 1;

    void *addr = allocate_mem_recursive(tree, size, half_size, chunk_start, left_index, height - 1);
    if (addr == NULL)
    {
        uint64_t right_index = node_index + _2pown(height - 1);
        addr = allocate_mem_recursive(tree, size, half_size, chunk_start + half_size, right_index, height - 1);
    }

    return addr;
}

void *mem_alloc(memoryManagerADT mem_manager, uint64_t size)
{
    uint64_t aux = (size & (size - 1)) == 0 ? size : _2pown(nlog2(size) + 1);
    uint64_t best_fit_size = max(aux, _2pown(MIN_BLOCK_SIZE_ORDER));
    void *out = allocate_mem_recursive(mem_manager->root, best_fit_size, mem_manager->memory_size, mem_manager->memory_start, 0, mem_manager->tree_max_height);
    mem_manager->free_memory_size -= (out != NULL) ? best_fit_size : 0;
    return out;
}

int is_valid_dir(memoryManagerADT mem_manager, void *address)
{
    if (address < mem_manager->memory_start || address >= mem_manager->memory_start + mem_manager->memory_size)
    {
        return 0;
    }

    uint64_t offset = address - mem_manager->memory_start;
    return (offset % _2pown(MIN_BLOCK_SIZE_ORDER) == 0);
}

int64_t free_mem_recursive(memoryNode *tree, void *address, uint64_t chunk_size, void *chunk_start, uint64_t node_index, uint64_t height)
{
    uint64_t half_size = chunk_size / 2;
    uint64_t left_index = node_index + 1;
    uint64_t right_index = node_index + _2pown(height - 1);

    int64_t out = 0;

    if (address == chunk_start)
    {
        if (tree[node_index].status != SPLIT)
        {
            out = (tree[node_index].status == USED) ? chunk_size : 0;
            tree[node_index].status = FREE;
            return out;
        }

        out = free_mem_recursive(tree, address, half_size, chunk_start, left_index, height - 1);
    }
    else
    {
        if (address < chunk_start)
            return -1;

        out = free_mem_recursive(tree, address, half_size, chunk_start + half_size, right_index, height - 1);

        if (out < 0)
        {
            out = free_mem_recursive(tree, address, half_size, chunk_start, left_index, height - 1);
        }
    }

    if (tree[left_index].status == FREE && tree[right_index].status == FREE)
        tree[node_index].status = FREE;

    return out;
}

void mem_free(memoryManagerADT mem_manager, void *address)
{
    if (!is_valid_dir(mem_manager, address))
        return;

    uint64_t freed_size = free_mem_recursive(mem_manager->root, address, mem_manager->memory_size, mem_manager->memory_start, 0, mem_manager->tree_max_height);
    mem_manager->free_memory_size += freed_size;
}

uint64_t nlog2(uint64_t n)
{
    uint64_t count;
    for (count = 0; n > 0; count++, n = n >> 1)
        ;
    return count - 1;
}

uint64_t _2pown(uint64_t n)
{
    uint64_t out;
    for (out = 1; n > 0; n--, out = out << 1)
        ;
    return out;
}
