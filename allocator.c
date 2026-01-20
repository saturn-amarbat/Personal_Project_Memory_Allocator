#include "allocator.h"
#include <stdio.h>
#include <stddef.h> // For size_t
#include <stdint.h> // For uintptr_t

#define HEAP_SIZE (1024 * 1024) // 1 MB
#define ALIGNMENT 8 // 8-byte alignment

// Helper macro to align size up to the next multiple of 8
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

// Define the BlockHeader structure
// In this simple implementation, 'next' points to the next block in physical memory.
typedef struct BlockHeader {
    size_t size;           // Size of the data payload (excluding header)
    struct BlockHeader* next; // Pointer to the next block in memory
    int is_free;           // 1 if free, 0 if allocated
} BlockHeader;

static char heap[HEAP_SIZE];
static BlockHeader* head = NULL;

void allocator_init(size_t size) {
    // Ensure the heap is aligned (implicitly handled by static array usually, but good practice)
    // We initialize the first block to cover the entire heap minus the header size.
    head = (BlockHeader*)heap;
    head->size = HEAP_SIZE - sizeof(BlockHeader);
    head->next = NULL;
    head->is_free = 1;

    printf("Allocator initialized. Heap size: %d bytes. Initial payload size: %zu bytes.\n", HEAP_SIZE, head->size);
}

void* s_malloc(size_t size) {
    if (size == 0) return NULL;

    size_t aligned_size = ALIGN(size);
    BlockHeader* current = head;

    while (current != NULL) {
        if (current->is_free && current->size >= aligned_size) {
            // Found a suitable block
            // Check if we can split it
            if (current->size >= aligned_size + sizeof(BlockHeader) + ALIGNMENT) {
                BlockHeader* new_block = (BlockHeader*)((char*)current + sizeof(BlockHeader) + aligned_size);
                new_block->size = current->size - aligned_size - sizeof(BlockHeader);
                new_block->next = current->next;
                new_block->is_free = 1;

                current->size = aligned_size;
                current->next = new_block;
            }
            
            current->is_free = 0;
            printf("Allocated %zu bytes at %p\n", aligned_size, (void*)(current + 1));
            return (void*)(current + 1); // Return pointer to payload (after header)
        }
        current = current->next;
    }

    printf("s_malloc failed: Out of memory for request of %zu bytes\n", size);
    return NULL;
}

void s_coalesce() {
    BlockHeader* current = head;
    while (current != NULL && current->next != NULL) {
        if (current->is_free && current->next->is_free) {
            // Merge current and next
            current->size += sizeof(BlockHeader) + current->next->size;
            current->next = current->next->next;
            // Don't advance current, need to check if the new next can also be merged
        } else {
            current = current->next;
        }
    }
}

void s_free(void* ptr) {
    if (ptr == NULL) return;

    // Pointer arithmetic to get the header from payload pointer
    BlockHeader* block = (BlockHeader*)ptr - 1;
    
    // Basic validation (optional but recommended)
    if ((char*)block < heap || (char*)block >= heap + HEAP_SIZE) {
        printf("s_free error: Invalid pointer %p\n", ptr);
        return;
    }

    block->is_free = 1;
    printf("Freed block at %p\n", ptr);

    // Coalesce adjacent free blocks to prevent fragmentation
    s_coalesce();
}