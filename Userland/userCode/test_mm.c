#include <test_utils.h>
#include <syscall_adapters.h>
#include <std.h>
#include <lib.h>

#define MAX_ITERS 100
#define MAX_BLOCKS 128

typedef struct MM_rq {
    void *address;
    uint32_t size;
} mm_rq;

uint64_t test_mm(char** argv, int argc) {
    mm_rq mm_rqs[MAX_BLOCKS];
    uint8_t rq;
    uint32_t total;

    uint64_t usable_memory = sys_get_usable_memory_size();
    printf("Usable memory: %ld\n", usable_memory);

    uint64_t max_memory;
    if (argc != 2 || (max_memory = satoi(argv[1])) <= 0) {
        puts_with_color("test_mm: ERROR must provide max_memory (tops at usable_memory / 2)\n", 0xFF0000);
        return -1;
    }

    uint64_t half_usable = usable_memory / 2;
    max_memory = (max_memory > half_usable) ? half_usable : max_memory;

    uint64_t count = 0;
    while (count++ < MAX_ITERS) {
        rq = 0;
        total = 0;
        uint64_t free_memory = sys_get_free_memory_size();

        puts_with_color("\n\nStart test iteration\n", 0xc2daff);
        printf("Free memory: %ld, Allocating memory...\n\n", free_memory);

        // Request as many blocks as we can
        while (rq < MAX_BLOCKS && total < max_memory && free_memory != 0) {
            mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
            mm_rqs[rq].address = sys_malloc(mm_rqs[rq].size);

            if (mm_rqs[rq].address) {
                free_memory = sys_get_free_memory_size();
                total += mm_rqs[rq].size;
                printf("Allocated memory: %d - Free memory: %ld, Ptr: %ld\n", mm_rqs[rq].size, free_memory, (uint64_t)mm_rqs[rq].address);
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
                    puts_with_color("test_mm ERROR\n", 0xFF0000);
                    return -1;
                } 
       
        // Free
        for (i = 0; i < rq; i++)
            if (mm_rqs[i].address)
                sys_free(mm_rqs[i].address);

        puts_with_color("-----------------------------------\n", 0xc2daff);
    }

    return 0;
}
