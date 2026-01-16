#include "allocator.h"
#include <stdio.h>
#include <stddef.h> // For size_t
#include <stdint.h> // For uintptr_t

#define HEAP_SIZE (1024 * 1024) // 1 MB

// Define the BlockHeader structure
typedef struct BlockHeader {
    size_t size;           // Size of the block (including header)
    struct BlockHeader* next; // Pointer to the next free block
    int is_free;           // 1 if free, 0 if allocated
} BlockHeader;

static char heap[HEAP_SIZE];
static BlockHeader* free_list_head = NULL;

void allocator_init(size_t size) {
    // Initialize the entire heap as one large free block
    free_list_head = (BlockHeader*)heap;
    free_list_head->size = HEAP_SIZE;
    free_list_head->next = NULL;
    free_list_head->is_free = 1;

    printf("Allocator initialized with heap size: %d bytes. Initial free block: %zu bytes.\n", HEAP_SIZE, free_list_head->size);
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


