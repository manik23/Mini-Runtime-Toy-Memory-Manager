#include "mini_malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ALLOCS 10000
#define MAX_SIZE 128

void run_benchmark() {
  struct timespec start, end;
  void *ptrs[NUM_ALLOCS];

  printf("--- Benchmarking Mini-Malloc ---\n");
  mini_init(1024 * 1024 * 10); // 10MB Arena

  clock_gettime(CLOCK_MONOTONIC, &start);

  // 1. Stress Allocation
  int success_count = 0;
  for (int i = 0; i < NUM_ALLOCS; i++) {
    size_t size = (rand() % MAX_SIZE) + 1;
    ptrs[i] = mini_malloc(size);
    if (ptrs[i])
      success_count++;
  }

  // 2. Stress Free
  for (int i = 0; i < NUM_ALLOCS; i++) {
    if (ptrs[i] && (i % 2 == 0)) { // Free 50% randomly
      mini_free(ptrs[i]);
      ptrs[i] = NULL;
    }
  }

  // 3. Re-allocation (testing reuse)
  for (int i = 0; i < NUM_ALLOCS; i++) {
    if (ptrs[i] == NULL) {
      size_t size = (rand() % MAX_SIZE) + 1;
      ptrs[i] = mini_malloc(size);
      if (ptrs[i])
        success_count++;
    }
  }

  clock_gettime(CLOCK_MONOTONIC, &end);

  double time_taken =
      (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

  printf("Allocations attempted: %d\n", NUM_ALLOCS * 2);
  printf("Successful allocs:    %d\n", success_count);
  printf("Time taken:           %.6f seconds\n", time_taken);
  printf("Avg time/op:          %.6f microseconds\n",
         (time_taken / (NUM_ALLOCS * 2)) * 1e6);

  mini_cleanup();
}

int main() {
  srand(time(NULL));
  run_benchmark();
  return 0;
}
