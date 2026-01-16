#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

/* Initialize the allocator.
 * In this fixed-size implementation, 'size' might be ignored if we use a static array,
 * or used to check bounds.
 */
void allocator_init(size_t size);

/* Allocate memory of 'size' bytes. Returns NULL on failure. */
void* s_malloc(size_t size);

/* Free the memory pointed to by 'ptr'. */
void s_free(void* ptr);

#endif
