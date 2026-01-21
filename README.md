# Custom Memory Allocator

A custom memory allocator implementation in C, created to explore low-level systems programming concepts.

## Features
- Fixed-size heap initialization (1 MB).
- **Best-Fit Allocation Strategy**: Efficiently finds the smallest available block that fits the requested size to minimize wasted space.
- **Block Splitting**: Minimizes fragmentation by splitting large free blocks during allocation.
- **Coalescing**: Merges adjacent free blocks upon deallocation to reduce external fragmentation.
- `BlockHeader` structure for memory management (tracks size and free status).

## Documentation
- [Design Document](docs/design.md): Explanation of the heap structure and Best-Fit algorithm.
- [Experiments Log](docs/experiments.md): Notes and learnings from the development process.
- [Changelog](CHANGELOG.md): Version history.

## Contributing
Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Build and Run
Use the Makefile to build the demo:
```bash
make
./allocator_demo
```

## Implementation Details
- **Alignment**: All allocations are aligned to 8-byte boundaries.
- **Metadata**: Each block is prefixed with a `BlockHeader` containing its size and status.
- **Heap**: A static 1MB character array is used as the heap.