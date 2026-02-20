# Mini-Runtime-Toy-Memory-Manager

A educational memory manager written in C using `mmap` to understand the internal workings of modern runtimes like Go (tcmalloc) and Java.

## Project Roadmap

| Phase | Milestone | Description | Status |
| :--- | :--- | :--- | :--- |
| **Phase 1** | [Arena & Bump Allocator](./exercises/phase1) | Basic `mmap` allocation and linear cursor. | ✅ Done |
| **Phase 2** | [Segregated Free Lists](./exercises/phase2) | Slab-style pages with reuse logic for small objects. | ✅ Done |
| **Phase 3** | [Page Heap & Large Objects](./exercises/phase3) | Support for dynamic growth and allocations > 2KB. | ✅ Done |
| **Phase 4** | [Explicit Free Lists](./exercises/phase4) | O(1) allocation using LIFO free lists. | ✅ Done |
| **Phase 5** | Coalescing & Fragmentation | Merging adjacent free blocks to reduce external fragmentation. | 🚧 In Progress |

## How to use
Check the `exercises/` directory for incremental solutions and descriptions of each step.

To run the current version:
```bash
make clean run
```
