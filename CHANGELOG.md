# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased]

## [0.4.0] - 2026-01-31
### Added
- **`s_strdup`**: Added a function to duplicate null-terminated strings.
- **`allocator_reset`**: Added a function to reset the heap to its initial state, freeing all memory.
- **Tests**: Added tests for `s_strdup` and `allocator_reset` in `main.c`.

## [0.3.0] - 2026-01-20
### Added
- **Memory Statistics**: Added `allocator_get_stats()` to retrieve heap usage metrics.
- **Demo Update**: `main.c` now prints a statistical summary at the end.

## [0.2.0] - 2026-01-20
### Changed
- **Allocation Strategy**: Switched from First-Fit to **Best-Fit** to reduce fragmentation.
- Updated documentation to reflect the new strategy.

## [0.1.0] - 2026-01-20
### Added
- **`s_calloc`**: Added support for zero-initialized memory allocation.
- **`s_realloc`**: Added support for resizing memory blocks.
- **Debug Tools**: Added `allocator_debug_print()` to visualize heap state.
- **Experiments Log**: Added `experiments.md` to track learning progress.
