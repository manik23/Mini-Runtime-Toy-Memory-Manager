#include "arena.h"
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

struct Arena *rootArena = NULL;

void arena_init(size_t size) {
  // We map the arena + 1 page for metadata (struct Arena)
  struct Arena *arena =
      (struct Arena *)mmap(NULL, size + PAGE_SIZE, PROT_READ | PROT_WRITE,
                           MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (arena == MAP_FAILED)
    return;

  arena->start_address = (void *)arena;
  arena->end_address = (void *)arena + size + PAGE_SIZE;
  arena->total_size = size + PAGE_SIZE;

  for (int i = 0; i < 9; i++) {
    // Each size class gets its own page. Page 0 is reserved for struct Arena.
    arena->free_lists[i].start_address = (char *)arena + PAGE_SIZE * (i + 1);
    arena->free_lists[i].end_address = (char *)arena + PAGE_SIZE * (i + 2);

    // Setup a dummy head block at the start of the page
    struct FreeBlock *dummy =
        (struct FreeBlock *)arena->free_lists[i].start_address;
    dummy->size = 8 << i;
    dummy->is_busy =
        1; // Dummy is always busy to prevent its data area from being used
    dummy->next = NULL;
    arena->free_lists[i].head = dummy;
  }
  rootArena = arena;
}

void *allocate_from_arena(size_t size) {
  // 1. Round to next power of two (8B minimum)
  size_t aligned = 8;
  while (aligned < size)
    aligned <<= 1;

  // 2. Find the correct list index
  int idx = 0;
  size_t temp_s = aligned;
  while (temp_s > 8) {
    temp_s >>= 1;
    idx++;
  }
  if (idx >= 9)
    return NULL; // Only handles up to 2048 bytes (8 << 8)

  struct lists *list = &rootArena->free_lists[idx];
  struct FreeBlock *curr = list->head;

  // 3. Search for existing free block (First Fit)
  while (curr) {
    if (!curr->is_busy && curr->size >= aligned) {
      curr->is_busy = 1;
      return (void *)(curr + 1);
    }
    if (!curr->next)
      break;
    curr = curr->next;
  }

  // 4. Carve new block if page has space
  // Next header starts after current Header + current Data
  struct FreeBlock *newB =
      (struct FreeBlock *)((char *)(curr + 1) + curr->size);

  // Check if both the Header AND the requested Data fit in the page
  if ((char *)newB + sizeof(struct FreeBlock) + aligned >
      (char *)list->end_address) {
    return NULL; // Page full for this size class
  }

  newB->size = aligned;
  newB->is_busy = 1;
  newB->next = NULL;
  curr->next = newB;

  return (void *)(newB + 1);
}

void free_from_arena(void *ptr) {
  if (!ptr)
    return;
  // Step back exactly one header size
  struct FreeBlock *header = (struct FreeBlock *)ptr - 1;
  header->is_busy = 0;
}

void arena_free(struct Arena *arena) {
  if (arena)
    munmap(arena->start_address, arena->total_size);
}
