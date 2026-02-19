#ifndef MINI_MALLOC_H
#define MINI_MALLOC_H

#include <stddef.h>

void mini_init(size_t size);
void mini_cleanup();
void *mini_malloc(size_t size);
void mini_free(void *ptr);
void mini_stats();

#endif