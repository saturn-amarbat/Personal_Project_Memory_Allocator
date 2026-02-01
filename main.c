#include "allocator.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for rand
#include <time.h>   // for time

void run_basic_tests() {
    printf("--- Running Basic Tests ---\n");
    allocator_init(0); 

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
    printf("\n[Test 4] Testing Calloc (5 ints)...");
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
    printf("\n[Test 5] Testing Realloc (expand p1 from 100 to 500 bytes)...");
    void* p1_new = s_realloc(p1, 500);
    if (p1_new) {
        printf("Realloc success. Old p1: %p, New p1: %p\n", p1, p1_new);
        p1 = p1_new;
    }

    allocator_debug_print();

    // Test 6: Free all and coalescing
    printf("\n[Test 6] Freeing all and coalescing...\n");
    s_free(p1);
    s_free(p3);
    s_free(p4);
    s_free(arr);

    allocator_debug_print();

    // Test 7: Realloc Optimization (Expand in place)
    printf("\n[Test 7] Testing Realloc Optimization (Expand into next free block)...");
    void* ptrA = s_malloc(100);
    void* ptrB = s_malloc(100); // Neighbor
    printf("Allocated ptrA: %p, ptrB: %p\n", ptrA, ptrB);
    
    printf("Freeing ptrB (neighbor)...");
    s_free(ptrB); // Now the space after ptrA is free
    
    printf("Reallocating ptrA to 150 bytes (should expand into ptrB's spot)...");
    void* ptrA_new = s_realloc(ptrA, 150);
    printf("New ptrA: %p\n", ptrA_new);
    
    if (ptrA == ptrA_new) {
        printf("SUCCESS: Realloc expanded in place! Optimization working.\n");
    } else {
        printf("NOTICE: Realloc moved the block. Optimization NOT implemented yet.\n");
    }
    s_free(ptrA_new);

    // Test 8: s_strdup
    printf("\n[Test 8] Testing s_strdup...\n");
    char* original_str = "Hello, Allocator!";
    char* dup_str = s_strdup(original_str);
    if (dup_str) {
        printf("Duplicated string: '%s' at %p\n", dup_str, (void*)dup_str);
        s_free(dup_str);
    }
}

void run_stress_test() {
    printf("\n--- Running Stress Test ---\n");
    allocator_reset();
    
    srand(time(NULL));
    void* ptrs[50];
    int count = 0;

    printf("Executing 50 random allocations/frees...\n");
    for (int i = 0; i < 50; i++) {
        if (count > 0 && (rand() % 3 == 0)) {
            // Randomly free
            int idx = rand() % count;
            s_free(ptrs[idx]);
            // Remove from list by swapping with last
            ptrs[idx] = ptrs[count-1];
            count--;
        } else {
            // Allocate
            size_t sz = (rand() % 256) + 1;
            void* p = s_malloc(sz);
            if (p) {
                ptrs[count++] = p;
            } else {
                printf("Allocation failed during stress test at step %d\n", i);
            }
        }
    }
    
    printf("Cleaning up remaining %d blocks...\n", count);
    for(int i=0; i<count; i++) {
        s_free(ptrs[i]);
    }
    
    allocator_debug_print();
}

int main() {
    printf("=== Custom Memory Allocator Demo ===\n");
    
    run_basic_tests();
    run_stress_test();

    printf("\n--- Final Statistics ---\n");
    AllocatorStats stats;
    allocator_get_stats(&stats);
    printf("Total Size: %zu\n", stats.total_size);
    printf("Used Memory: %zu\n", stats.used_memory);
    printf("Free Memory: %zu\n", stats.free_memory);
    printf("Used Blocks: %zu\n", stats.used_blocks);
    printf("Free Blocks: %zu\n", stats.free_blocks);
    printf("Largest Free Block: %zu\n", stats.largest_free_block);

    return 0;
}