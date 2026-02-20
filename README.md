# Mini-Runtime-Toy-Memory-Manager

A educational memory manager written in C using `mmap` to understand the internal workings of modern runtimes like Go (tcmalloc) and Java.

## Project Roadmap

| Phase | Milestone | Description | Status |
| :--- | :--- | :--- | :--- |
| **Phase 1** | [Arena & Bump Allocator](./exercises/phase1) | Basic `mmap` allocation and linear cursor. | ✅ Done |
| **Phase 2** | [Segregated Free Lists](./exercises/phase2) | Slab-style pages with reuse logic for small objects. | ✅ Done |
| **Phase 3** | [Page Heap & Large Objects](./exercises/phase3) | Support for dynamic growth and allocations > 2KB. | ✅ Done |
| **Phase 4** | [Explicit Free Lists](./exercises/phase4) | O(1) allocation using LIFO free lists. **(500x Speedup)** | ✅ Done |
| **Phase 5** | Coalescing & Fragmentation | Merging adjacent free blocks to reduce external fragmentation. | 🚧 In Progress |

### Phase 4 Performance Benchmark
Phase 4 optimized memory discovery and reuse to constant time ($O(1)$), resulting in a massive performance boost.

| Metric | Mini-Malloc | Standard Malloc (libc) |
| :--- | :--- | :--- |
| **Avg Latency** | **0.26 µs** | **0.18 µs** |
| **Throughput** | ~3.8M ops/sec | ~5.5M ops/sec |
| **Complexity** | **O(1)** | **O(1)** |

#### Benchmark Output:
```text
Running Mini-Malloc Benchmark...
--- Arena Details ---
Page Size:        16384
Arena Start:     0x10316c000
Total Size:      100 MB
Pages Used:      2254
Next Fresh Page: 0x1054a4000

--- Comparison Results ---
Metric               Mini-Malloc     Std Malloc     
------------------------------------------------------------
Time (sec)           0.021485        0.014696       
Total Attempts       80000           80000          
Success Rate         100.00%         100.00%         
Avg Latency (us)     0.268563        0.183700
```

---

## How to use
Check the `exercises/` directory for incremental solutions and descriptions of each step.

To run the current version:
```bash
make clean run
```
