#include "arena.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h> // mmap, munmap
#include <unistd.h>   // getpagesize

void mini_init(size_t size) { arena_init(ARENA_SIZE); }

void mini_stats() {
  size_t total_size = rootArena->total_size;
  printf("Total Bytes: %zu\n", total_size);
  printf("page size: %d\n", PAGE_SIZE);
  //   printf("Bytes Free: %zu\n", byte_free);
  //   printf("Bytes Used: %zu\n", total_size - byte_free);
}

void *allocate_from_arena(size_t size) {
  // Ensure 8-byte alignment for the next allocation
  size = (size + 7) & ~7;

  int free_list_index = log2(size) - 3;
  printf("free list index: %d\n", free_list_index);

  struct FreeBlock *block = rootArena->free_lists[free_list_index];
  struct FreeBlock *prev = NULL;
  while (block->next != NULL &&
         block->next != rootArena->free_lists[free_list_index] + PAGE_SIZE) {
    prev = block;
    block = block->next;
  }

  if (block->next == rootArena->free_lists[free_list_index] + PAGE_SIZE) {
    printf("No free block found in size %zu\n", size);
    return NULL;
  }

  block->next = NULL;
  block->prev = prev;
  block->size = size;

  return block;
}

void *mini_malloc(size_t size) {
  if (rootArena == NULL) {
    mini_init(ARENA_SIZE);
  }

  return allocate_from_arena(size);
}

void mini_cleanup() { arena_free(rootArena); }

void mini_free(void *ptr) {}