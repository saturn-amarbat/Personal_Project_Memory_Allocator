# allocator experiments log

## concepts to explore
- [ ] **Alignment**: how do i ensure pointers are 8-byte aligned?
- [ ] **Fragmentation**: what happens when i free a small block between two allocated ones?
- [ ] **Coalescing**: how do i merge adjacent free blocks?

## questions
- is `sizeof(BlockHeader)` exactly what i expect (struct padding)?
- how do i iterate through the linked list safely?
