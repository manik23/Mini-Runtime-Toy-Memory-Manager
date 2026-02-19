#include "arena.h"
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

struct Arena *rootArena = NULL;

void arena_init(size_t size) {
  struct Arena *arena = (struct Arena *)mmap(
      NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (arena == MAP_FAILED)
    return;

  arena->start_address = (void *)arena;
  arena->end_address = (void *)arena + size;
  arena->total_size = size;

  for (int i = 0; i < 9; i++) {
    // Skip first page (metadata)
    arena->free_lists[i].start_address = (char *)arena + PAGE_SIZE * (i + 1);
    arena->free_lists[i].end_address = (char *)arena + PAGE_SIZE * (i + 2);

    struct FreeBlock *dummy =
        (struct FreeBlock *)arena->free_lists[i].start_address;
    dummy->size = 8 << i;
    dummy->is_busy = 1; // Dummy is always "busy"
    dummy->next = NULL;
    arena->free_lists[i].head = dummy;
  }
  rootArena = arena;
}

void *allocate_from_arena(size_t size) {
  size_t aligned = 8;
  while (aligned < size)
    aligned <<= 1;

  int idx = 0;
  size_t s = aligned;
  while (s > 8) {
    s >>= 1;
    idx++;
  }
  if (idx >= 9)
    return NULL;

  struct lists *list = &rootArena->free_lists[idx];
  struct FreeBlock *curr = list->head;

  while (curr) {
    if (!curr->is_busy && curr->size >= aligned) {
      curr->is_busy = 1;
      return (void *)(curr + 1);
    }
    if (!curr->next)
      break;
    curr = curr->next;
  }

  struct FreeBlock *newB =
      (struct FreeBlock *)((char *)(curr + 1) + curr->size);
  if ((char *)newB + sizeof(struct FreeBlock) + aligned >
      (char *)list->end_address) {
    return NULL;
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
  struct FreeBlock *header = (struct FreeBlock *)ptr - 1;
  header->is_busy = 0;
}

void arena_free(struct Arena *arena) {
  if (arena)
    munmap(arena->start_address, arena->total_size);
}
