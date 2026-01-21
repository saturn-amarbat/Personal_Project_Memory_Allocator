# allocator experiments log

## concepts explored
- [x] **Alignment**: Pointers are aligned to 8-byte boundaries using the `ALIGN` macro.
- [x] **Fragmentation**: Observed how freeing a middle block creates a hole that can be reused by smaller or equal allocations.
- [x] **Coalescing**: Implemented merging of adjacent free blocks in `s_free` to reduce external fragmentation.
- [x] **Zero Initialization**: Added `s_calloc` to ensure memory is cleared.
- [x] **Resizing**: Added `s_realloc` to handle dynamic resizing (move-and-copy strategy).

## questions & answers
- **is `sizeof(BlockHeader)` exactly what i expect (struct padding)?**
  - Yes, `sizeof(size_t)` is usually 8, `sizeof(pointer)` is 8, `sizeof(int)` is 4. Struct size is likely 24 bytes (padded to align to 8 bytes) or packed differently depending on compiler. In this implementation, we align payloads, so header size impact is constant overhead.
- **how do i iterate through the linked list safely?**
  - By checking `current != NULL` loop conditions.
- **How to debug the heap?**
  - Added `allocator_debug_print` to dump the list state.

## future improvements
- [ ] **Best Fit**: Implement best fit strategy to reduce fragmentation further.
- [ ] **Thread Safety**: Add mutex locking for thread-safe operations.
- [ ] **Shrink in Realloc**: Optimize `s_realloc` to support in-place shrinking.