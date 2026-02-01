#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize the allocator.
 * In this fixed-size implementation, 'size' might be ignored if we use a static array,
 * or used to check bounds.
 */
void allocator_init(size_t size);

/* Allocate memory of 'size' bytes. Returns NULL on failure. */
void* s_malloc(size_t size);

/* Free the memory pointed to by 'ptr'. */
void s_free(void* ptr);

/* Allocate memory for an array of 'num' elements of 'size' bytes each and initialize to zero. */
void* s_calloc(size_t num, size_t size);

/* Resize the memory block pointed to by 'ptr' to 'size' bytes. */
void* s_realloc(void* ptr, size_t size);

/* Duplicate a null-terminated string. Returns NULL on failure. */
char* s_strdup(const char* s);

/* Reset the allocator, freeing all memory. */
void allocator_reset();

/* Print the current state of the heap (debug). */
void allocator_debug_print();

typedef struct {
    size_t total_size;
    size_t used_memory;
    size_t free_memory;
    size_t used_blocks;
    size_t free_blocks;
    size_t largest_free_block;
} AllocatorStats;

/* Populate the stats struct with current heap metrics. */
void allocator_get_stats(AllocatorStats* stats);

#ifdef __cplusplus
}
#endif

#endif
