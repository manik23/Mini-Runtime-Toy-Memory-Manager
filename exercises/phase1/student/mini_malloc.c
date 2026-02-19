#include "mini_malloc.h"
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

typedef struct {
  void *start;
  void *current;
  void *end;
  size_t total_size;
} Arena;

static Arena global_arena;

void mini_init(size_t size) {
  void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (ptr == MAP_FAILED) {
    perror("mmap");
    return;
  }
  global_arena.start = ptr;
  global_arena.current = ptr;
  global_arena.end = (char *)ptr + size;
  global_arena.total_size = size;
}

void *mini_malloc(size_t size) {
  // 8-byte alignment
  size_t aligned_size = (size + 7) & ~7;

  if ((char *)global_arena.current + aligned_size > (char *)global_arena.end) {
    return NULL;
  }

  void *ptr = global_arena.current;
  global_arena.current = (char *)global_arena.current + aligned_size;
  return ptr;
}

void mini_free(void *ptr) {
  // Phase 1: Bump allocator cannot free individual blocks
  (void)ptr;
}

void mini_stats() {
  size_t used = (char *)global_arena.current - (char *)global_arena.start;
  printf("--- Phase 1 Stats ---\n");
  printf("Total: %zu bytes\n", global_arena.total_size);
  printf("Used:  %zu bytes\n", used);
  printf("Free:  %zu bytes\n", global_arena.total_size - used);
}
