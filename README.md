# Mini-Runtime-Toy-Memory-Manager

A educational memory manager written in C using `mmap` to understand the internal workings of modern runtimes like Go (tcmalloc) and Java.

## Project Roadmap

| Phase | Milestone | Description | Status |
| :--- | :--- | :--- | :--- |
| **Phase 1** | [Arena & Bump Allocator](./exercises/phase1) | Basic `mmap` allocation and linear cursor. | ✅ Done |
| **Phase 2** | [Segregated Free Lists](./exercises/phase2) | Slab-style pages with reuse logic for small objects. | ✅ Done |
| **Phase 3** | Page Heap & Large Objects | Support for dynamic growth and allocations > 2KB. | 🚧 In Progress |
| **Phase 4** | Coalescing & Fragmentation | Merging adjacent free blocks to reduce external fragmentation. | ⏳ Planned |
| **Phase 5** | Garbage Collection | Mark-and-sweep implementation. | ⏳ Planned |

## How to use
Check the `exercises/` directory for incremental solutions and descriptions of each step.

To run the current version:
```bash
make clean run
```
