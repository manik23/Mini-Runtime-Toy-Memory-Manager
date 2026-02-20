#ifndef MINI_MALLOC_H
#define MINI_MALLOC_H

#include <stddef.h>

void mini_stats();
void mini_cleanup();
void *mini_malloc(size_t size);
void mini_free(void *ptr);

#endif