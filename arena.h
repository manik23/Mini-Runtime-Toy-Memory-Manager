#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>
#include <unistd.h>

#define PAGE_SIZE getpagesize()
#define ARENA_SIZE (16 * PAGE_SIZE)

struct FreeBlock {
  size_t size;
  struct FreeBlock *next;
  struct FreeBlock *prev;
};

struct Arena {
  void *start_address;
  void *end_address;
  struct FreeBlock *free_lists[9];
  size_t total_size;
};

extern struct Arena *rootArena;

void arena_init(size_t size);
void arena_free(struct Arena *arena);
void display_arena(struct Arena *arena);

#endif