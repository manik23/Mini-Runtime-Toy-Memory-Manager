# Phase 4: Explicit Free Lists & O(1) Speed

### Goal
Optimize the allocator from $O(N)$ linear search to $O(1)$ constant-time allocation and discovery.

### Key Concepts
- **LIFO Explicit Free List**: Maintaining a logical "stack" of available blocks to bypass busy sectors during allocation.
- **Ghost Pointers (Payload Reuse)**: Storing the `free_next` pointer inside the user's data area of a free block to achieve zero metadata overhead for the free list.
- **Physical Tail Tracking**: Adding a `tail` pointer to the slab list to enable instant $O(1)$ "carving" of new blocks without scanning the entire page.
- **Micro-Optimization**: Achieving latencies comparable to `malloc()` (reducing from ~100µs to ~0.2µs).

### Benchmark Results (50,000 Ops, 100MB Arena)
This phase achieved a **~500x speedup** over Phase 3 by eliminating linear scans.

| Metric | Mini-Malloc (Phase 4) | Standard Malloc (libc) |
| :--- | :--- | :--- |
| **Total Ops** | 80,000 | 80,000 |
| **Avg Latency** | **0.268 µs** | **0.183 µs** |
| **Success Rate** | 100% | 100% |

> **Note**: Latency in Phase 3 was **~140 µs**. The transition to $O(1)$ discovery and reuse brought us within the same order of magnitude as production allocators.

### Implementation Details
- **Free Path**: When a block is freed, it is pushed onto the `free_head` stack. The old `free_head` pointer is written into the first 8 bytes of the freed block's data area.
- **Allocation Path**:
    1. Check `free_head`. If not NULL, "pop" the block (Fast Path).
    2. If `free_head` is NULL, check `tail`. If the block at `tail` is not busy, use it.
    3. If `tail` is busy, carve a new block after it.

### How to run
```bash
make clean run
make benchmark
```
