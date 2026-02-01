# Custom Memory Allocator

![Status](https://img.shields.io/badge/status-active-success.svg)
![License](https://img.shields.io/badge/license-MIT-blue.svg)

A custom memory allocator implementation in C, created to explore low-level systems programming concepts.

## Features
- **Fixed-size Heap**: 1 MB static buffer.
- **Best-Fit Strategy**: Selects the smallest sufficient free block to reduce fragmentation.
- **Coalescing**: Merges adjacent free blocks immediately on `s_free`.
- **In-Place Realloc**: Expands into adjacent free blocks when possible to avoid data copy.
- **Standard API**: `s_malloc`, `s_free`, `s_calloc`, `s_realloc`, `s_strdup`.
- **Debug Tools**: Helper function to dump heap state.

## Documentation
- [Design Document](docs/design.md): Explanation of the heap structure and Best-Fit algorithm.
- [Experiments Log](docs/experiments.md): Notes and learnings from the development process.
- [Changelog](CHANGELOG.md): Version history.

## Contributing
Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Build and Run
Use the provided Makefile to build and run the demo:
```bash
make
./allocator_demo
```

## Implementation Details
- **Alignment**: All allocations are aligned to 8-byte boundaries.
- **Metadata**: Each block is prefixed with a `BlockHeader` containing its size and status.
- **Heap**: A static 1MB character array is used as the heap.
- **Realloc Optimization**: `s_realloc` is smart! It checks if the adjacent next block is free and just expands into it instead of moving data around. Saves time and reduces fragmentation.