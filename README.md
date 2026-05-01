# Custom Memory Allocator

[![Language](https://img.shields.io/badge/C-Systems%20Programming-blue?style=flat-square&logo=c)](#)
[![Status](https://img.shields.io/badge/status-complete-success.svg?style=flat-square)](#)
[![License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square)](LICENSE)

A custom `malloc`/`free` implementation in C, built from scratch as a systems portfolio project. Manages a fixed 1MB heap without any standard library memory functions — all pointer arithmetic, block metadata, and coalescing are hand-rolled.

This is the kind of project most developers never write because high-level languages abstract it away. Building it forces you to understand exactly what happens beneath `malloc`.

---

## 🧠 Technical Highlights (For Recruiters)

| Concept | Implementation |
|---|---|
| **Manual Memory Management** | Custom heap via 1MB static buffer; no stdlib malloc used |
| **Best-Fit Allocation** | Scans free list to find smallest sufficient block, reducing fragmentation |
| **Block Coalescing** | Adjacent free blocks merged immediately on `s_free` |
| **In-Place Realloc** | Expands into adjacent free block instead of copying data when possible |
| **Pointer Arithmetic** | All block navigation done via raw pointer math and `BlockHeader` metadata |
| **8-Byte Alignment** | All allocations aligned to 8-byte boundaries (matches real-world allocator requirements) |

> **Skills:** C, Systems Programming, Heap Management, Memory Allocator Design, Pointer Arithmetic, malloc/free

---

## ⚙️ API

```c
void  allocator_init();                    // Initialize the 1MB heap
void* s_malloc(size_t size);               // Allocate block (best-fit)
void  s_free(void* ptr);                   // Free block + coalesce neighbors
void* s_calloc(size_t n, size_t size);     // Zero-initialized allocation
void* s_realloc(void* ptr, size_t size);   // Resize (in-place if possible)
char* s_strdup(const char* str);           // String duplication
```

---

## 🚀 Build and Run

```bash
make
./allocator_demo
```

---

## 🔧 Implementation Details

- **Alignment**: All allocations are aligned to 8-byte boundaries.
- **Metadata**: Each block is prefixed with a `BlockHeader` containing its size and status.
- **Heap**: A static 1MB character array is used as the heap.
- **Realloc Optimization**: `s_realloc` checks if the adjacent next block is free and expands into it instead of moving data. Saves time and reduces fragmentation.
- **Debug Tools**: Helper function to dump heap state for inspection.

---

## 📚 Documentation

- [Design Document](docs/design.md): Explanation of the heap structure and Best-Fit algorithm.
- [Experiments Log](docs/experiments.md): Notes and learnings from the development process.
- [Changelog](CHANGELOG.md): Version history.

---

## 📜 License

MIT License — see [LICENSE](LICENSE) for details.
