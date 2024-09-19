#include "./../include/managers/memoryManager.h"
#include <stdio.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define MIN_BLOCK_SIZE_ORDER 8 // min_block_size = 2^order
//#define NULL 0

uint64_t nlog2(uint64_t n);
uint64_t _2pown(uint64_t n);

typedef enum { FREE, SPLIT, USED } memoryNodeStatus;

typedef struct memoryNode{
    memoryNodeStatus status;
} memoryNode;

struct memoryManagerCDT {
    memoryNode* root;
    void* memory_start;
    void* tree_memory_start;
    uint64_t tree_max_height;
    uint64_t memory_size;
};

memoryManagerADT init_memory_manager(void *memory, uint64_t memory_size) {
    uint64_t max_block_order = nlog2(memory_size);
    uint64_t tree_max_height = max_block_order - MIN_BLOCK_SIZE_ORDER + 1;
    uint64_t tree_max_node_count = _2pown(tree_max_height) - 1;
    uint64_t tree_max_size = sizeof(memoryNode) * tree_max_node_count;

    if (memory_size < tree_max_size) return NULL;

    uint64_t usable_memory = _2pown(nlog2(memory_size - tree_max_size));

    memoryManagerADT mem_manager = memory;
    mem_manager->tree_memory_start = memory + sizeof(struct memoryManagerCDT);
    mem_manager->memory_start = mem_manager->tree_memory_start + tree_max_size;
    mem_manager->memory_size = usable_memory;
    mem_manager->tree_max_height = tree_max_height;

    memoryNode* root = memory + sizeof(struct memoryManagerCDT);
    root->status = FREE;
    mem_manager->root = root;

    return mem_manager;
}

void* allocate_mem_recursive(memoryNode* tree, uint64_t size, uint64_t chunck_size, void* memory_start, uint64_t node_index, uint64_t height) {
    if (chunck_size < size || tree[node_index].status == USED) 
        return NULL;

    if (chunck_size == size) {
        if (tree[node_index].status == SPLIT) 
            return NULL;

        tree[node_index].status = USED;
        return memory_start;
    }

    if (tree[node_index].status != SPLIT) {
        tree[node_index].status = SPLIT;
    }

    uint64_t half_size = chunck_size / 2;
    uint64_t left_index = node_index + 1;

    void* addr = allocate_mem_recursive(tree, size, half_size, memory_start, left_index, height - 1);
    if (addr == NULL) {
        uint64_t right_index = node_index + _2pown(height - 1); 
        addr = allocate_mem_recursive(tree, size, half_size, memory_start + half_size, right_index, height - 1);
    }

    return addr;
}

void* mem_alloc(memoryManagerADT mem_manager, uint64_t size) {
    uint64_t best_fit_size = max(_2pown(nlog2(size) + 1), _2pown(MIN_BLOCK_SIZE_ORDER));
    //printf("total usable_memory: %ld\tbest size: %ld\n", mem_manager->memory_size, best_fit_size);
    return allocate_mem_recursive(mem_manager->root, best_fit_size, mem_manager->memory_size, mem_manager->memory_start, 0, mem_manager->tree_max_height);
}

void mem_free(memoryManagerADT mem_manager, void* address) {

}

uint64_t nlog2(uint64_t n) {
    uint64_t count;
    for (count = 0; n > 0; count++, n = n >> 1) ;
    return count - 1;
}

uint64_t _2pown(uint64_t n) {
    uint64_t out;
    for (out = 1; n > 0; n--, out = out << 1) ;
    return out;
}
