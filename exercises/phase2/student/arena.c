#include "arena.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h> // mmap, munmap
#include <unistd.h>   // getpagesize

struct Arena *rootArena = NULL;

void arena_init(size_t size) {
  struct Arena *arena = (struct Arena *)mmap(
      NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

  if (arena == MAP_FAILED) {
    perror("mmap");
    return;
  }

  arena->start_address = (void *)arena;
  arena->end_address = (void *)arena + size;

  for (int i = 0; i < 9; i++) {
    // printf("Free list index %d: \n", i);
    arena->free_lists[i].start_address = (char *)arena + PAGE_SIZE * (i + 1);
    arena->free_lists[i].end_address = (char *)arena + PAGE_SIZE * (i + 2);
    // printf("start address: %p\n", arena->free_lists[i].start_address);
    // printf("end address: %p\n", arena->free_lists[i].end_address);

    arena->free_lists[i].head =
        (struct FreeBlock *)arena->free_lists[i].start_address;
    arena->free_lists[i].head->next = NULL;
    arena->free_lists[i].head->prev = NULL;
    arena->free_lists[i].head->size = 8 << i;
    arena->free_lists[i].head->is_busy = 0;
  }
  arena->total_size = size;
  rootArena = arena;
}

void *allocate_from_arena(size_t size) {

  if (rootArena == NULL) {
    printf("Arena not initialized\n");
    return NULL;
  }

  // 1. Round to next power of two (8B minimum)
  size_t aligned = 8;
  while (aligned < size)
    aligned <<= 1;

  size = aligned;

  // 2. Find the correct list index
  int slab = 0;
  while (aligned > 8) {
    aligned >>= 1;
    slab++;
  }

  // 3. Only handles up to 2048 bytes (8 << 8)
  if (slab >= 9)
    return NULL;

  printf("requested size: %zu\n at slab: %d\n", size, slab);

  struct lists *list = &rootArena->free_lists[slab];

  struct FreeBlock *curr = list->head;

  while (curr->is_busy && curr->next != NULL) {
    curr = curr->next;
  }

  if (!curr->is_busy) {
    curr->is_busy = 1;
    return (void *)(curr + 1);
  }

  // Carve a new block after curr
  struct FreeBlock *temp =
      (struct FreeBlock *)((char *)(curr + 1) + curr->size);

  // Safety check: Ensure the new block Header AND at least 1 unit of data fit
  // in the page
  if ((char *)temp + sizeof(struct FreeBlock) + size >
      (char *)list->end_address) {
    printf("No more space in list %d\n", slab);
    return NULL;
  }

  temp->size = size;
  temp->is_busy = 1;
  temp->next = NULL;
  temp->prev = curr;
  curr->next = temp;

  return (void *)(temp + 1);
}

void free_from_arena(void *ptr) {

  if (ptr == NULL) {
    return;
  }

  struct FreeBlock *block = (struct FreeBlock *)ptr - 1;
  block->is_busy = 0;
  printf("Freed block at %p (Header at %p)\n", ptr, (void *)block);
}

void arena_free(struct Arena *arena) {
  if (arena == NULL) {
    return;
  }
  printf("Freeing arena at address: %p of size: %zu\n", arena,
         arena->total_size);
  munmap(arena->start_address, arena->total_size);
  arena = NULL;
}

void display_arena(struct Arena *arena) {
  printf("\n--- Arena Details ---\n");
  printf("Page Size: %d\n", PAGE_SIZE);
  printf("Arena: %p\n", arena);
  printf("Start Address: %p\n", arena->start_address);
  printf("End Address: %p\n", arena->end_address);
}