#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>
#include <unistd.h>

#define PAGE_SIZE getpagesize()

struct FreeBlock {
  size_t size;
  int is_busy;
  struct FreeBlock *next;
};

struct lists {
  struct FreeBlock *head;
  void *start_address;
  void *end_address;
};

struct Arena {
  void *start_address;
  void *end_address;
  struct lists free_lists[9];
  size_t total_size;
};

extern struct Arena *rootArena;

void arena_init(size_t size);
void *allocate_from_arena(size_t size);
void free_from_arena(void *ptr);
void arena_free(struct Arena *arena);

#endif
