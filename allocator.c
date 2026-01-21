#include "allocator.h"
#include <stdio.h>
#include <stddef.h> // For size_t
#include <stdint.h> // For uintptr_t
#include <string.h> // For memset, memcpy

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
    BlockHeader* best_fit = NULL;

    // Best-Fit Search: Find the smallest free block that fits
    while (current != NULL) {
        if (current->is_free && current->size >= aligned_size) {
            if (best_fit == NULL || current->size < best_fit->size) {
                best_fit = current;
                // Optimization: Exact match found, stop searching
                if (current->size == aligned_size) break;
            }
        }
        current = current->next;
    }

    if (best_fit != NULL) {
        // Check if we can split the best fit block
        if (best_fit->size >= aligned_size + sizeof(BlockHeader) + ALIGNMENT) {
            BlockHeader* new_block = (BlockHeader*)((char*)best_fit + sizeof(BlockHeader) + aligned_size);
            new_block->size = best_fit->size - aligned_size - sizeof(BlockHeader);
            new_block->next = best_fit->next;
            new_block->is_free = 1;

            best_fit->size = aligned_size;
            best_fit->next = new_block;
        }
        
        best_fit->is_free = 0;
        printf("Allocated %zu bytes at %p\n", aligned_size, (void*)(best_fit + 1));
        return (void*)(best_fit + 1); // Return pointer to payload (after header)
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

void* s_calloc(size_t num, size_t size) {
    if (num == 0 || size == 0) return NULL;
    
    size_t total_size = num * size;
    // Check for overflow (basic)
    if (total_size / num != size) return NULL;

    void* ptr = s_malloc(total_size);
    if (ptr != NULL) {
        memset(ptr, 0, total_size);
        printf("Calloc zeroed %zu bytes at %p\n", total_size, ptr);
    }
    return ptr;
}

void* s_realloc(void* ptr, size_t size) {
    if (ptr == NULL) {
        return s_malloc(size);
    }
    if (size == 0) {
        s_free(ptr);
        return NULL;
    }

    BlockHeader* block = (BlockHeader*)ptr - 1;
    size_t old_size = block->size;

    if (ALIGN(size) <= old_size) {
        // Shrinking or same size (ignoring explicit shrink support for now)
        return ptr; 
    }

    // Allocate new block
    void* new_ptr = s_malloc(size);
    if (new_ptr != NULL) {
        // Copy old data
        memcpy(new_ptr, ptr, old_size);
        // Free old block
        s_free(ptr);
        printf("Realloc moved data from %p to %p\n", ptr, new_ptr);
    } else {
        printf("Realloc failed to allocate new block\n");
    }

    return new_ptr;
}

void allocator_debug_print() {
    BlockHeader* current = head;
    printf("\n--- Heap State ---\n");
    while (current != NULL) {
        printf("Block at %p: Size=%zu, Free=%d, Next=%p\n", 
               (void*)current, current->size, current->is_free, (void*)current->next);
        current = current->next;
    }
    printf("------------------\n\n");
}
