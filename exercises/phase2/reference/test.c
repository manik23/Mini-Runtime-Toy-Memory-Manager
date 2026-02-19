#include "mini_malloc.h"
#include <stdio.h>

int main() {
  printf("--- Phase 2 Reference Test ---\n");
  mini_init(256 * 1024); // 256KB Arena

  printf("Allocating p1 (8 bytes)...\n");
  void *p1 = mini_malloc(8);
  printf("p1: %p\n", p1);

  printf("Freeing p1...\n");
  mini_free(p1);

  printf("Allocating p2 (8 bytes)...\n");
  void *p2 = mini_malloc(8);
  printf("p2: %p\n", p2);

  if (p1 == p2) {
    printf("SUCCESS: p1 was reused for p2!\n");
  } else {
    printf("FAILURE: p2 did not reuse p1's address (%p != %p)\n", p1, p2);
  }

  printf("Allocating p3 (16 bytes)...\n");
  void *p3 = mini_malloc(16);
  printf("p3: %p\n", p3);

  mini_cleanup();
  printf("Test complete.\n");
  return 0;
}
