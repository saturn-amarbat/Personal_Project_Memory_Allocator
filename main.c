#include "allocator.h"
#include <stdio.h>

int main() {
    printf("--- Custom Memory Allocator Demo ---\n");
    allocator_init(0); // Size arg is ignored in this fixed-size implementation

    // Test 1: Basic Allocation
    printf("\n[Test 1] Allocating 3 small blocks...\n");
    void* p1 = s_malloc(100);
    void* p2 = s_malloc(200);
    void* p3 = s_malloc(300);

    if (p1 && p2 && p3) printf("Success: p1=%p, p2=%p, p3=%p\n", p1, p2, p3);
    else printf("Failure: Some allocations returned NULL\n");

    // Test 2: Freeing middle block
    printf("\n[Test 2] Freeing p2 (middle block)...\n");
    s_free(p2);

    // Test 3: Reuse freed space
    printf("\n[Test 3] Allocating 150 bytes (should fit in p2's spot)...");
    void* p4 = s_malloc(150);
    printf("p4 allocated at %p (Expected: Same as old p2)\n", p4);

    // Test 4: Free all and coalesce
    printf("\n[Test 4] Freeing p1, p3, p4 and coalescing...\n");
    s_free(p1);
    s_free(p3);
    s_free(p4);

    // Test 5: Allocate large block
    printf("\n[Test 5] Allocating large block (500 KB)...");
    void* p5 = s_malloc(500 * 1024);
    if (p5) printf("Success: p5 allocated at %p\n", p5);
    else printf("Failure: Could not allocate large block\n");
    
    s_free(p5);

    printf("\n--- Demo Completed ---\n");
    return 0;
}
