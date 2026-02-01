# Design Document

## Architecture Overview

This project implements a fixed-size custom memory allocator in C. It manages a static heap using a linked list of memory blocks.

### Memory Layout

The heap is a contiguous array of bytes (`static char heap[HEAP_SIZE]`). It is managed by traversing a linked list of **Block Headers**.

```
+----------------+----------------+----------------+----------------+
| Block Header 1 |    Payload 1   | Block Header 2 |    Payload 2   | ...
+----------------+----------------+----------------+----------------+
```

### Block Header Structure

Each memory block is prefixed by a header defined as:

```c
typedef struct BlockHeader {
    size_t size;              // Size of the payload (excluding header)
    struct BlockHeader* next; // Pointer to the next block
    int is_free;              // Status flag (1 = Free, 0 = Allocated)
} BlockHeader;
```

### Allocation Strategy: Best-Fit

We use the **Best-Fit** algorithm to minimize external fragmentation.

1.  **Search**: Iterate through the entire linked list.
2.  **Compare**: For every free block that fits the requested size, record the one with the smallest size difference.
3.  **Select**: Choose the "tightest" fit.
4.  **Split**: If the chosen block is significantly larger than requested, split it into two:
    *   One allocated block (requested size).
    *   One new free block (remaining size).

### Coalescing (Deallocation)

When `s_free()` is called:
1.  Mark the block as free.
2.  **Coalesce**: Scan the list; if two free blocks are adjacent, merge them into a single larger block. This prevents the heap from becoming fragmented into many tiny unusable free blocks.

### Diagrams

**Splitting a Block:**
```
Before:
[ Header |           Free Space (Large)           ]

After Allocation:
[ Header | Allocated ] [ Header | New Free Space  ]
```

**Coalescing Blocks:**
```
Before:
[ Header | Free ] [ Header | Free ]

After Free:
[ Header |            Merged Free Space           ]
```

### Reallocation Strategy

`s_realloc` handles resizing memory blocks with an optimization to reduce data movement:

1.  **Shrink/Same Size**: If the new size is smaller or equal, do nothing (return the same pointer).
2.  **Expand In-Place**: Check if the *next* block in memory is free.
    *   If `current_block_size + next_block_size` >= `new_size`, merge them and occupy the necessary space.
    *   This avoids expensive `memcpy` operations and keeps the heap cleaner.
3.  **Move**: If in-place expansion isn't possible, `malloc` a new block, `memcpy` data, and `free` the old block.

### Utility Functions

- **`s_calloc`**: Allocates memory and initializes it to zero.
- **`s_strdup`**: Allocates memory and duplicates a string into it.
- **`allocator_reset`**: Resets the internal pointers to treating the entire heap as one large free block.

