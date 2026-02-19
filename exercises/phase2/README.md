# Phase 2: Segregated Free Lists (Slabs)

### Goal
Implement memory reclamation and reuse using segregated lists divided into pages.

### Solutions
- **[Student Solution](./student)**: Your implementation using a "Dummy Head" linked list per size class.
- **[Reference Solution](./reference)**: A clean implementation featuring power-of-two rounding and linked-list reuse.

### Concepts Covered
- **Size Classes**: Grouping allocations (8B, 16B, 32B...).
- **Explicit Free Lists**: Tracking free blocks via in-memory headers.
- **Pointer Arithmetic**: Moving from user data back to the header during `mini_free`.
