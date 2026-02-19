#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>
#include <unistd.h>

#define PAGE_SIZE getpagesize()
#define ARENA_SIZE (10L * 1024L * 1024L)

struct FreeBlock {
  struct FreeBlock *next;
  struct FreeBlock *prev;
  size_t size;
  int is_busy;
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
  void *next_fresh_page;
  int current_page;
  size_t total_size;
};

extern struct Arena *rootArena;

void *allocate_from_arena(size_t size);
void free_from_arena(void *ptr);
void arena_free(struct Arena *arena);
void display_arena(struct Arena *arena);

#endif