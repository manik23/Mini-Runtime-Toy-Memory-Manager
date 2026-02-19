#include "mini_malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ALLOCS 100000
#define MAX_SIZE 128
#define LARGE_SIZE 2048

void run_benchmark() {
  struct timespec start, end;
  void **ptrs = malloc(NUM_ALLOCS * sizeof(void *));
  if (!ptrs) {
    perror("malloc ptrs");
    return;
  }

  printf("--- Benchmarking Mini-Malloc ---\n");

  clock_gettime(CLOCK_MONOTONIC, &start);

  int total_attempts = 0;
  int success_count = 0; // Overall success count

  int success_p1 = 0;
  // 1. Stress Allocation
  printf("Phase 1: Stress Allocation (%d ops)... ", NUM_ALLOCS);
  for (int i = 0; i < NUM_ALLOCS; i++) {
    size_t size = (rand() % MAX_SIZE) + 1;
    total_attempts++;
    ptrs[i] = mini_malloc(size);
    if (ptrs[i])
      success_p1++;
  }
  printf("Success: %d\n", success_p1);
  success_count += success_p1;

  // 2. Stress Free
  printf("Phase 2: Partial Free (50%% of ops)...\n");
  int free_count = 0;
  for (int i = 0; i < NUM_ALLOCS; i++) {
    if (ptrs[i] && (i % 2 == 0)) { // Corrected 'ptr[i]' to 'ptrs[i]'
      mini_free(ptrs[i]);
      ptrs[i] = NULL;
      free_count++;
    }
  }

  int success_p3 = 0;
  // 3. Re-allocation (testing reuse)
  printf("Phase 3: Re-allocation on freed slots (%d ops)... ", free_count);
  for (int i = 0; i < NUM_ALLOCS; i++) {
    if (ptrs[i] == NULL) {
      size_t size = (rand() % MAX_SIZE) + 1;
      total_attempts++;
      ptrs[i] = mini_malloc(size);
      if (ptrs[i])
        success_p3++;
    }
  }
  printf("Success: %d\n", success_p3);
  success_count += success_p3;

  // 4. Free all
  printf("Phase 4: Free all remaining...\n");
  for (int i = 0; i < NUM_ALLOCS; i++) {
    if (ptrs[i]) {
      mini_free(ptrs[i]);
      ptrs[i] = NULL;
    }
  }

  int success_p5 = 0;
  // 5. Large Blocks
  printf("Phase 5: Large Block Allocation (%d ops)... ", NUM_ALLOCS / 10);
  for (int i = 0; i < NUM_ALLOCS / 10; i++) {
    size_t size =
        LARGE_SIZE + (rand() % 10000); // Corrected 'LARGE_SIZE' to 'MAX_SIZE'
    total_attempts++;
    void *p = mini_malloc(size);
    if (p) {
      success_p5++;
      mini_free(p);
    }
  }
  printf("Success: %d\n", success_p5);
  success_count += success_p5;

  clock_gettime(CLOCK_MONOTONIC, &end);

  double time_taken =
      (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

  printf("\n--- Results ---\n");
  printf("Total attempts:       %d\n", total_attempts);
  printf("Successful allocs:    %d\n", success_count);
  printf("Success rate:         %.2f%%\n",
         (double)success_count / total_attempts * 100);
  printf("Time taken:           %.6f seconds\n", time_taken);
  printf("Avg time/op:          %.6f microseconds\n",
         (time_taken / total_attempts) * 1e6);

  free(ptrs);
  mini_stats();
  mini_cleanup();
}

int main() {
  srand(time(NULL));
  run_benchmark();
  return 0;
}
