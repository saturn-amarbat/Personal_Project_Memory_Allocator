#include "allocator.h"
#include <stdio.h>
#include <stddef.h>

#define HEAP_SIZE (1024 * 1024) // 1 MB

static char heap[HEAP_SIZE];
// TODO: Add metadata structures for free list management

void allocator_init(size_t size) {
    printf("Allocator initialized with heap size: %d bytes\n", HEAP_SIZE);
    // TODO: Initialize free list
}

void* s_malloc(size_t size) {
    printf("s_malloc requested: %zu bytes\n", size);
    // TODO: Implement allocation strategy (e.g., First-Fit)
    return NULL; // Placeholder
}

void s_free(void* ptr) {
    printf("s_free called on ptr: %p\n", ptr);
    // TODO: Implement free logic and coalescing
}

