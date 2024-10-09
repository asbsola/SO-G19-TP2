#include <test_util.h>
#include <managers/memoryManager.h>
#include <stdio.h>
#include <string.h>

#define MAX_ITERS 8000
#define MAX_BLOCKS 128

typedef struct MM_rq {
    void *address;
    uint32_t size;
} mm_rq;

uint64_t test_mm(uint64_t argc, char *argv[]) {
    mm_rq mm_rqs[MAX_BLOCKS];
    uint8_t rq;
    uint32_t total;
    uint64_t max_memory = 10000;

    if (argc != 1)
        return -1;

    if ((max_memory = satoi(argv[0])) <= 0)
        return -1;

    char memory[max_memory];
    memoryManagerADT mem_manager = init_memory_manager(memory, max_memory);

    if (mem_manager == NULL) return -1;

    max_memory = get_usable_memory_size(mem_manager);
    printf("Usable memory: %ld\n", max_memory);

    uint64_t counter = 0;

    while (counter++ < MAX_ITERS) {
        rq = 0;
        total = 0;
        uint64_t free_memory = get_free_memory_size(mem_manager);

        printf("\n\nFree memory: %ld, Allocating memory...\n\n", free_memory);

        // Request as many blocks as we can
        while (rq < MAX_BLOCKS && total < max_memory && free_memory != 0) {
            mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
            mm_rqs[rq].address = mem_alloc(mem_manager, mm_rqs[rq].size);

            if (mm_rqs[rq].address) {
                free_memory = get_free_memory_size(mem_manager);
                total += mm_rqs[rq].size;
                printf("Memoria allocada: %d - Memoria libre: %ld\n", mm_rqs[rq].size, free_memory);
                rq++;
            }
        }

        // Set
        uint32_t i;
        for (i = 0; i < rq; i++)
            if (mm_rqs[i].address)
                memset(mm_rqs[i].address, i, mm_rqs[i].size);

        // Check
        for (i = 0; i < rq; i++)
            if (mm_rqs[i].address)
                if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
                    printf("test_mm ERROR\n");
                    return -1;
                } 
       
        // Free
        for (i = 0; i < rq; i++)
            if (mm_rqs[i].address)
                mem_free(mem_manager, mm_rqs[i].address);
    }
    return 0;
}

int main(int argc, char** argv) {
    int status = test_mm(argc - 1, argv + 1);
    printf("Status: %d\n", status);
    return 0;
}
