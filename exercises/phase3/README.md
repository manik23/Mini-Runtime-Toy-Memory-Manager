# Phase 3: The Page Heap (Growth & Hybrid Path)

### Goal
Handle allocations larger than 2KB and implement dynamic arena expansion ("Page Stealing") to prevent premature Out-of-Memory (OOM) errors.

### Key Concepts
- **Hybrid Allocator**: Routing requests to different paths based on size (Slabs vs. Direct mmap).
- **Dynamic Paging**: Allowing slab lists to grow by claiming new pages from a centralized pool.
- **Lazy Initialization**: Automatically setting up the arena on the first `mini_malloc` call.
- **Resource Management**: Automatically unmapping large objects while marking small ones as available for reuse.

### Student Solution
The student's live implementation now handles up to 10MB of memory dynamically and supports huge allocations (e.g., 100KB) through a dedicated large-object path.

### Reference Solution
The reference solution contains the "Gold Standard" implementation of the page-stealing logic and the large object branching seen in this phase.

### How to run
```bash
make clean run
make benchmark
```
