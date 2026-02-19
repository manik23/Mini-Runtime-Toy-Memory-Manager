#include "mini_malloc.h"
#include <stdio.h>

int main() {
  mini_init(1024 * 1024); // 1MB Arena

  printf("Allocating 8 bytes...\n");
  int *p1 = mini_malloc(8);
  *p1 = 42;
  printf("p1 at %p, value: %d\n", p1, *p1);

  printf("Allocating 100 bytes...\n");
  void *p2 = mini_malloc(100);
  printf("p2 at %p\n", p2);

  mini_stats();
  return 0;
}
