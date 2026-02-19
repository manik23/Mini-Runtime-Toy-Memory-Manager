#include "arena.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h> // mmap, munmap
#include <unistd.h>   // getpagesize

void mini_stats() { display_arena(rootArena); }

void *mini_malloc(size_t size) { return allocate_from_arena(size); }

void mini_cleanup() { arena_free(rootArena); }

void mini_free(void *ptr) { free_from_arena(ptr); }