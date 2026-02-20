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
  arena->current_page++;

  for (int i = 0; i < 9; i++) {
#ifdef DEBUG
    // printf("Free list index %d: \n", i);
#endif
    arena->free_lists[i].start_address = (char *)arena + PAGE_SIZE * (i + 1);
    arena->free_lists[i].end_address = (char *)arena + PAGE_SIZE * (i + 2);
#ifdef DEBUG
    // printf("start address: %p\n", arena->free_lists[i].start_address);
    // printf("end address: %p\n", arena->free_lists[i].end_address);
#endif

    arena->free_lists[i].head =
        (struct FreeBlock *)arena->free_lists[i].start_address;
    arena->free_lists[i].head->next = NULL;
    arena->free_lists[i].head->prev = NULL;
    arena->free_lists[i].head->size = 8 << i;
    arena->free_lists[i].head->is_busy = 0;
    arena->free_lists[i].head->slab_index = i;
    arena->free_lists[i].free_head = NULL;
    arena->free_lists[i].tail = arena->free_lists[i].head;
    arena->current_page++;
  }
  arena->total_size = size;
  arena->next_fresh_page =
      (char *)arena->start_address + PAGE_SIZE * arena->current_page;
#ifdef DEBUG
  printf("total used pages: %d and next free page at: %p\n",
         arena->current_page, arena->next_fresh_page);
#endif
  rootArena = arena;
}

void *allocate_from_arena(size_t size) {
  if (size == 0) {
    printf("Size is 0\n");
    return NULL;
  }

  if (size > ARENA_SIZE) {
    printf("Size is greater than arena size\n");
    return NULL;
  }

  if (rootArena == NULL) {
    arena_init(ARENA_SIZE);
  }

  if (size > (8 << 8)) {
#ifdef DEBUG
    printf("Allocating larger block of size %zu\n", size);
#endif
    struct FreeBlock *larger_ptr = (struct FreeBlock *)mmap(
        NULL, size + sizeof(struct FreeBlock), PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (larger_ptr == MAP_FAILED) {
      perror("mmap large block");
      return NULL;
    }

    larger_ptr->size = size;
    larger_ptr->is_busy = 1;
    larger_ptr->next = NULL;
    larger_ptr->prev = NULL;
    larger_ptr->slab_index = -1;
    return (void *)(larger_ptr + 1);
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

#ifdef DEBUG
  printf("requested size: %zu at slab: %d\n", size, slab);
#endif

  struct lists *list = &rootArena->free_lists[slab];

  if (list->free_head != NULL) {
    // Ghost Pointer (Payload) Trick
    struct FreeBlock *found = list->free_head;
    list->free_head = *(struct FreeBlock **)(found + 1);
    found->is_busy = 1;
    return (void *)(found + 1);
  }

  struct FreeBlock *tail = list->tail;

  if (!tail->is_busy) {
    tail->is_busy = 1;
    return (void *)(tail + 1);
  }

  // Carve a new block after tail
  struct FreeBlock *temp =
      (struct FreeBlock *)((char *)(tail + 1) + tail->size);

  // Safety check: Ensure the new block Header AND at least 1 unit of data fit
  // in the page
  if ((char *)temp + sizeof(struct FreeBlock) + size >
      (char *)list->end_address) {

    if (rootArena->next_fresh_page + PAGE_SIZE <= rootArena->end_address) {

      // 1. Check if we have any fresh pages left in our arena
#ifdef DEBUG
      printf("Slab %d full. Stealing fresh page at %p\n", slab,
             rootArena->next_fresh_page);
#endif
      void *new_page = rootArena->next_fresh_page;
      rootArena->next_fresh_page += PAGE_SIZE;
      rootArena->current_page++;

      // 2. Turn this new page into a new FreeBlock linked to the chain
      struct FreeBlock *fresh_block = (struct FreeBlock *)new_page;
      fresh_block->is_busy = 1;
      fresh_block->size = size;
      fresh_block->next = NULL;
      fresh_block->prev = tail;
      tail->next = fresh_block;
      fresh_block->slab_index = slab;

      // 3. update the list end address
      list->end_address = (char *)new_page + PAGE_SIZE;
      list->tail = fresh_block;

      return (void *)(fresh_block + 1);
    }

    return NULL;
  }

  temp->size = size;
  temp->is_busy = 1;
  temp->next = NULL;
  temp->prev = tail;
  tail->next = temp;
  temp->slab_index = slab;
  list->tail = temp;

  return (void *)(temp + 1);
}

void free_from_arena(void *ptr) {

  if (ptr == NULL) {
    return;
  }

  struct FreeBlock *block = (struct FreeBlock *)ptr - 1;

  if (block->size > (8 << 8)) {
#ifdef DEBUG
    printf("Will unmap block at %p (Header at %p)\n", ptr, (void *)block);
#endif
    munmap(block, block->size + sizeof(struct FreeBlock));
    return;
  } else {
#ifdef DEBUG
    printf("Will Reuse block at %p (Header at %p)\n", ptr, (void *)block);
#endif
    block->is_busy = 0;
    *(struct FreeBlock **)(block + 1) =
        rootArena->free_lists[block->slab_index].free_head;
    rootArena->free_lists[block->slab_index].free_head = block;
  }
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
  if (!arena)
    return;
  printf("\n--- Arena Details ---\n");
  printf("Page Size:        %d\n", PAGE_SIZE);
  printf("Arena Start:     %p\n", arena->start_address);
  printf("Arena End:       %p\n", arena->end_address);
  printf("Total Size:      %zu MB\n", arena->total_size / (1024 * 1024));
  printf("Pages Used:      %d\n", arena->current_page);
  printf("Next Fresh Page: %p\n", arena->next_fresh_page);
}