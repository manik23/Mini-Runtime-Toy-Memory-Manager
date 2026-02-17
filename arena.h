#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>
#include <unistd.h>

#define PAGE_SIZE getpagesize()
#define ARENA_SIZE (256 * PAGE_SIZE)

struct Arena {
  void *start_address;
  void *end_address;
  void *current_address;
  struct Arena *next;
  struct Arena *prev;
  int is_free;
  size_t total_size;
};

extern struct Arena *rootArena;

void arena_init(size_t size);
void arena_free(struct Arena *arena);
void display_arena(struct Arena *arena);

#endif