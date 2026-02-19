#include "mini_malloc.h"
#include "arena.h"

void mini_init(size_t size) { arena_init(size); }
void *mini_malloc(size_t size) { return allocate_from_arena(size); }
void mini_free(void *ptr) { free_from_arena(ptr); }
void mini_cleanup() { arena_free(rootArena); }
void mini_stats() { /* Stats implementation */
}
