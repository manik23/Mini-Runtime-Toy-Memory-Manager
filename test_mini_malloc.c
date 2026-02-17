#include "mini_malloc.h"
#include <stdio.h>

int main() {

  printf("Allocating first block (8 bytes)...\n");
  int *ptr1 = (int *)mini_malloc(8 * sizeof(char));

  if (ptr1 != NULL) {
    printf("Allocated ptr1 at: %p\n", (void *)ptr1);
    *ptr1 = 42;
    printf("Value at ptr1: %d (Success)\n", *ptr1);
  } else {
    fprintf(stderr, "error in memory allocation!\n");
    goto cleanup;
  }

cleanup:
  mini_stats();
  mini_cleanup();
  printf("Test complete.\n");
  return 0;
}
