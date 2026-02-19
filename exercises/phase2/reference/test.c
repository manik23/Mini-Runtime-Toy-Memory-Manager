#include "mini_malloc.h"
#include <stdio.h>

int main() {
  mini_init(1024 * 1024);

  void *p1 = mini_malloc(8);
  printf("p1: %p\n", p1);
  mini_free(p1);

  void *p2 = mini_malloc(8);
  printf("p2: %p (reused?)\n", p2);

  mini_cleanup();
  return 0;
}
