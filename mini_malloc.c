#include "arena.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h> // mmap, munmap
#include <unistd.h>   // getpagesize

void mini_stats() {
  size_t total_size = rootArena->total_size;
  printf("Total Bytes: %zu\n", total_size);
  printf("page size: %d\n", PAGE_SIZE);
  //   printf("Bytes Free: %zu\n", byte_free);
  //   printf("Bytes Used: %zu\n", total_size - byte_free);
}

void *mini_malloc(size_t size) { return allocate_from_arena(size); }

void mini_cleanup() { arena_free(rootArena); }

void mini_free(void *ptr) { free_from_arena(ptr); }