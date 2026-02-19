#include "mini_malloc.h"
#include <stdio.h>

int main() {

  printf("Allocating first 8 byte block (8 bytes)...\n");
  int *ptr1 = (int *)mini_malloc(8 * sizeof(char));

  if (ptr1 != NULL) {
    printf("Allocated ptr1 at: %p\n", (void *)ptr1);
    *ptr1 = 42;
    printf("Value at ptr1: %d (Success)\n", *ptr1);
  } else {
    fprintf(stderr, "error in memory allocation!\n");
    goto cleanup;
  }

  printf("Allocating second 8 byte block (8 bytes)...\n");
  int *ptr11 = (int *)mini_malloc(8 * sizeof(char));

  if (ptr11 != NULL) {
    printf("Allocated ptr11 at: %p\n", (void *)ptr11);
    *ptr11 = 43;
    printf("Value at ptr11: %d (Success)\n", *ptr11);
  } else {
    fprintf(stderr, "error in memory allocation!\n");
    goto cleanup;
  }

  printf("Allocating third 8 byte block (8 bytes)...\n");
  int *ptr111 = (int *)mini_malloc(8 * sizeof(char));

  if (ptr111 != NULL) {
    printf("Allocated ptr111 at: %p\n", (void *)ptr111);
    *ptr111 = 44;
    printf("Value at ptr111: %d (Success)\n", *ptr111);
  } else {
    fprintf(stderr, "error in memory allocation!\n");
    goto cleanup;
  }
  mini_free(ptr111);

  void *p1 = mini_malloc(8);
  printf("P1: %p\n", p1);
  mini_free(p1);
  void *p2 = mini_malloc(8);
  printf("P2: %p\n", p2); // P2 SHOULD be the same address as P1!
  mini_free(p2);

  while (1) {
    printf("Allocating  8 byte block (8 bytes)...\n");
    void *p = mini_malloc(8);
    if (p == NULL) {
      printf("Failed to allocate memory\n");
      break;
    }
    printf("P: %p\n", p);
  }

  while (1) {
    printf("Allocating  16 byte block (16 bytes)...\n");
    void *p = mini_malloc(16);
    if (p == NULL) {
      printf("Failed to allocate memory\n");
      break;
    }
    printf("P: %p\n", p);
  }

  while (1) {
    printf("Allocating  32 byte block (32 bytes)...\n");
    void *p = mini_malloc(32);
    if (p == NULL) {
      printf("Failed to allocate memory\n");
      break;
    }
    printf("P: %p\n", p);
  }

  while (1) {
    printf("Allocating  64 byte block (64 bytes)...\n");
    void *p = mini_malloc(64);
    if (p == NULL) {
      printf("Failed to allocate memory\n");
      break;
    }
    printf("P: %p\n", p);
  }

  printf("Allocating first block (130 bytes)...\n");
  int *ptr6 = (int *)mini_malloc(130 * sizeof(char));

  if (ptr6 != NULL) {
    printf("Allocated ptr6 at: %p\n", (void *)ptr6);
    *ptr6 = 42;
    printf("Value at ptr6: %d (Success)\n", *ptr6);
  } else {
    fprintf(stderr, "error in memory allocation!\n");
    goto cleanup;
  }

  printf("Allocating first block (281 bytes)...\n");
  int *ptr7 = (int *)mini_malloc(281 * sizeof(char));

  if (ptr7 != NULL) {
    printf("Allocated ptr7 at: %p\n", (void *)ptr7);
    *ptr7 = 42;
    printf("Value at ptr7: %d (Success)\n", *ptr7);
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
