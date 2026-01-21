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
