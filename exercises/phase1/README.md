# Phase 1: The Arena & Bump Allocator

### Goal
Establish a "Heap" by requesting a large contiguous block of memory from the kernel using `mmap`.

### Solutions
- **[Student Solution](./student)**: Your initial implementation using a simple cursor.
- **[Reference Solution](./reference)**: A clean, struct-based arena with metadata safety.

### Concepts Covered
- `mmap` vs `malloc`.
- Alignment (8-byte).
- Why bump allocators are O(1) but cannot free.
