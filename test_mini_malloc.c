#include "mini_malloc.h"
#include <stdio.h>

int main() {

  void *p1 = mini_malloc(8);
  printf("P1: %p\n", p1);
  mini_free(p1);
  void *p2 = mini_malloc(8);
  printf("P2: %p\n", p2); // P2 SHOULD be the same address as P1!
  mini_free(p2);

  printf("Allocating large block (100KB)...\n");
  void *large_p = mini_malloc(100 * 1024);
  if (large_p != NULL) {
    printf("Allocated large block at: %p\n", large_p);
    char *data = (char *)large_p;
    data[0] = 'H';
    data[1024] = 'i';
    printf("Large block verified (%c%c). Freeing it now...\n", data[0],
           data[1024]);
    mini_free(large_p);
  }

  mini_stats();
  mini_cleanup();
  printf("Test complete.\n");
  return 0;
}
