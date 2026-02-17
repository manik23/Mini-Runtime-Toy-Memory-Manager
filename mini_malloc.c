#include "arena.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h> // mmap, munmap
#include <unistd.h>   // getpagesize

void mini_init(size_t size) { arena_init(ARENA_SIZE); }

void mini_stats() {
  size_t byte_free = rootArena->end_address - rootArena->current_address;
  size_t total_size = rootArena->total_size;
  printf("Total Bytes: %zu\n", total_size);
  printf("Bytes Free: %zu\n", byte_free);
  printf("Bytes Used: %zu\n", total_size - byte_free);
}

void *allocate_from_arena(size_t size) {
  // Ensure 8-byte alignment for the next allocation
  size = (size + 7) & ~7;

  if ((char *)rootArena->current_address + size <=
      (char *)rootArena->end_address) {
    void *ptr = rootArena->current_address;
    rootArena->current_address = (char *)rootArena->current_address + size;
    return ptr;
  }
  return NULL;
}

void *mini_malloc(size_t size) {
  if (rootArena == NULL) {
    mini_init(ARENA_SIZE);
  }

  return allocate_from_arena(size);
}

void mini_cleanup() { arena_free(rootArena); }

void mini_free(void *ptr) {}