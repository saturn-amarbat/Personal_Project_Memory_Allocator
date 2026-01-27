#include "allocator.h"
#include <stdio.h>
#include <string.h>

int main() {
    // Entry point for the memory allocator demonstration
    printf("--- Custom Memory Allocator Demo ---\n");
    allocator_init(0); // Size arg is ignored in this fixed-size implementation

    // Test 1: Basic Allocation
    printf("\n[Test 1] Allocating 3 small blocks...\n");
    void* p1 = s_malloc(100);
    void* p2 = s_malloc(200);
    void* p3 = s_malloc(300);

    if (p1 && p2 && p3) printf("Success: p1=%p, p2=%p, p3=%p\n", p1, p2, p3);
    else printf("Failure: Some allocations returned NULL\n");

    allocator_debug_print();

    // Test 2: Freeing middle block
    printf("\n[Test 2] Freeing p2 (middle block)...\n");
    s_free(p2);
    
    allocator_debug_print();

    // Test 3: Reuse freed space
    printf("\n[Test 3] Allocating 150 bytes (should fit in p2's spot)...");
    void* p4 = s_malloc(150);
    printf("p4 allocated at %p (Expected: Same as old p2)\n", p4);

    // Test 4: Calloc
    printf("\n[Test 4] Testing Calloc (5 ints)...\n");
    int* arr = (int*)s_calloc(5, sizeof(int));
    if (arr) {
        printf("Calloc success. Checking zero initialization...\n");
        int is_zero = 1;
        for(int i=0; i<5; i++) {
            if (arr[i] != 0) is_zero = 0;
        }
        if (is_zero) printf("Verified: Array is zero-initialized.\n");
        else printf("Error: Array is NOT zero-initialized.\n");
    }

    // Test 5: Realloc
    printf("\n[Test 5] Testing Realloc (expand p1 from 100 to 500 bytes)...\n");
    // p1 is currently allocated. p2 was freed but p4 took part of it.
    // Let's reallocate p1. It should move because it's trapped by p4.
    void* p1_new = s_realloc(p1, 500);
    if (p1_new) {
        printf("Realloc success. Old p1: %p, New p1: %p\n", p1, p1_new);
        p1 = p1_new;
    }

    allocator_debug_print();

    // Test 6: Free all and coalesce
    printf("\n[Test 6] Freeing all and coalescing...\n");
    s_free(p1);
    s_free(p3);
    s_free(p4);
    s_free(arr);

    allocator_debug_print();

    printf("\n--- Final Statistics ---\n");
    AllocatorStats stats;
    allocator_get_stats(&stats);
    printf("Total Size: %zu\n", stats.total_size);
    printf("Used Memory: %zu\n", stats.used_memory);
    printf("Free Memory: %zu\n", stats.free_memory);
    printf("Used Blocks: %zu\n", stats.used_blocks);
    printf("Free Blocks: %zu\n", stats.free_blocks);
    printf("Largest Free Block: %zu\n", stats.largest_free_block);

    printf("\n--- Demo Completed ---\n");
    return 0;
}