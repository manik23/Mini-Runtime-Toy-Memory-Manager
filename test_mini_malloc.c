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

  printf("Allocating first block (8 bytes)...\n");
  int *ptr11 = (int *)mini_malloc(8 * sizeof(char));

  if (ptr11 != NULL) {
    printf("Allocated ptr1 at: %p\n", (void *)ptr1);
    *ptr11 = 42;
    printf("Value at ptr1: %d (Success)\n", *ptr1);
  } else {
    fprintf(stderr, "error in memory allocation!\n");
    goto cleanup;
  }

  printf("Allocating first block (8 bytes)...\n");
  int *ptr111 = (int *)mini_malloc(8 * sizeof(char));

  if (ptr111 != NULL) {
    printf("Allocated ptr1 at: %p\n", (void *)ptr1);
    *ptr111 = 42;
    printf("Value at ptr1: %d (Success)\n", *ptr1);
  } else {
    fprintf(stderr, "error in memory allocation!\n");
    goto cleanup;
  }

  //   printf("Allocating first block (16 bytes)...\n");
  //   int *ptr2 = (int *)mini_malloc(16 * sizeof(char));

  //   if (ptr2 != NULL) {
  //     printf("Allocated ptr1 at: %p\n", (void *)ptr1);
  //     *ptr2 = 42;
  //     printf("Value at ptr1: %d (Success)\n", *ptr2);
  //   } else {
  //     fprintf(stderr, "error in memory allocation!\n");
  //     goto cleanup;
  //   }

  //   printf("Allocating first block (32 bytes)...\n");
  //   int *ptr3 = (int *)mini_malloc(32 * sizeof(char));

  //   if (ptr3 != NULL) {
  //     printf("Allocated ptr1 at: %p\n", (void *)ptr1);
  //     *ptr3 = 42;
  //     printf("Value at ptr1: %d (Success)\n", *ptr3);
  //   } else {
  //     fprintf(stderr, "error in memory allocation!\n");
  //     goto cleanup;
  //   }

  //   printf("Allocating first block (25 bytes)...\n");
  //   int *ptr4 = (int *)mini_malloc(25 * sizeof(char));

  //   if (ptr4 != NULL) {
  //     printf("Allocated ptr1 at: %p\n", (void *)ptr1);
  //     *ptr4 = 42;
  //     printf("Value at ptr1: %d (Success)\n", *ptr4);
  //   } else {
  //     fprintf(stderr, "error in memory allocation!\n");
  //     goto cleanup;
  //   }

  //   printf("Allocating first block (65 bytes)...\n");
  //   int *ptr5 = (int *)mini_malloc(65 * sizeof(char));

  //   if (ptr5 != NULL) {
  //     printf("Allocated ptr1 at: %p\n", (void *)ptr1);
  //     *ptr5 = 42;
  //     printf("Value at ptr1: %d (Success)\n", *ptr5);
  //   } else {
  //     fprintf(stderr, "error in memory allocation!\n");
  //     goto cleanup;
  //   }

  //   printf("Allocating first block (130 bytes)...\n");
  //   int *ptr6 = (int *)mini_malloc(130 * sizeof(char));

  //   if (ptr6 != NULL) {
  //     printf("Allocated ptr1 at: %p\n", (void *)ptr1);
  //     *ptr6 = 42;
  //     printf("Value at ptr1: %d (Success)\n", *ptr5);
  //   } else {
  //     fprintf(stderr, "error in memory allocation!\n");
  //     goto cleanup;
  //   }

cleanup:
  mini_stats();
  mini_cleanup();
  printf("Test complete.\n");
  return 0;
}
