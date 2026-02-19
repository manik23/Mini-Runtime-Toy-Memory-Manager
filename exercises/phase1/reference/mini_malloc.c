#include "mini_malloc.h"
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

typedef struct {
  void *memory;
  size_t size;
  size_t offset;
} Arena;

static Arena *global_arena = NULL;

void mini_init(size_t size) {
  void *ptr = mmap(NULL, size + sizeof(Arena), PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

  if (ptr == MAP_FAILED)
    return;

  global_arena = (Arena *)ptr;
  global_arena->memory = (char *)ptr + sizeof(Arena);
  global_arena->size = size;
  global_arena->offset = 0;
}

void *mini_malloc(size_t size) {
  if (!global_arena)
    return NULL;

  size_t aligned_size = (size + 7) & ~7;

  if (global_arena->offset + aligned_size > global_arena->size) {
    return NULL;
  }

  void *ptr = (char *)global_arena->memory + global_arena->offset;
  global_arena->offset += aligned_size;

  return ptr;
}

void mini_free(void *ptr) { (void)ptr; }

void mini_stats() {
  if (!global_arena)
    return;
  printf("--- Reference Phase 1 Stats ---\n");
  printf("Used: %zu / %zu bytes\n", global_arena->offset, global_arena->size);
}
