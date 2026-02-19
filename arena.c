#include "arena.h"
#include <stdio.h>
#include <stdlib.h>
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
    arena->free_lists[i] =
        (struct FreeBlock *)(arena->start_address + i * PAGE_SIZE +
                             sizeof(struct FreeBlock));
    arena->free_lists[i]->next = NULL;
    arena->free_lists[i]->prev = NULL;
    arena->free_lists[i]->size = (8 << i);
    printf("Address of class size  %d: %p\n", (8 << i), arena->free_lists[i]);
  }
  arena->total_size = size;
  rootArena = arena;
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