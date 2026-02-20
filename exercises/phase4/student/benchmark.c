#include "mini_malloc.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ALLOCS 50000
#define MAX_SIZE 1024
#define LARGE_SIZE 2048

typedef struct {
  double time_taken;
  int total_attempts;
  int success_count;
} BenchmarkResult;

BenchmarkResult run_test(bool use_mini) {
  struct timespec start, end;
  void **ptrs = malloc(NUM_ALLOCS * sizeof(void *));
  if (!ptrs) {
    perror("malloc ptrs");
    exit(1);
  }

  int total_attempts = 0;
  int success_count = 0;

  clock_gettime(CLOCK_MONOTONIC, &start);

  // 1. Stress Allocation
  for (int i = 0; i < NUM_ALLOCS; i++) {
    size_t size = (rand() % MAX_SIZE) + 1;
    total_attempts++;
    ptrs[i] = use_mini ? mini_malloc(size) : malloc(size);
    if (ptrs[i])
      success_count++;
  }

  // 2. Partial Free (50%)
  for (int i = 0; i < NUM_ALLOCS; i++) {
    if (ptrs[i] && (i % 2 == 0)) {
      if (use_mini)
        mini_free(ptrs[i]);
      else
        free(ptrs[i]);
      ptrs[i] = NULL;
    }
  }

  // 3. Re-allocation
  for (int i = 0; i < NUM_ALLOCS; i++) {
    if (ptrs[i] == NULL) {
      size_t size = (rand() % MAX_SIZE) + 1;
      total_attempts++;
      ptrs[i] = use_mini ? mini_malloc(size) : malloc(size);
      if (ptrs[i])
        success_count++;
    }
  }

  // 4. Large Blocks
  for (int i = 0; i < NUM_ALLOCS / 10; i++) {
    size_t size = LARGE_SIZE + (rand() % 10000);
    total_attempts++;
    void *p = use_mini ? mini_malloc(size) : malloc(size);
    if (p) {
      success_count++;
      if (use_mini)
        mini_free(p);
      else
        free(p);
    }
  }

  // 5. Final Cleanup
  for (int i = 0; i < NUM_ALLOCS; i++) {
    if (ptrs[i]) {
      if (use_mini)
        mini_free(ptrs[i]);
      else
        free(ptrs[i]);
    }
  }

  clock_gettime(CLOCK_MONOTONIC, &end);
  free(ptrs);

  BenchmarkResult res;
  res.time_taken =
      (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
  res.total_attempts = total_attempts;
  res.success_count = success_count;
  return res;
}

int main() {
  srand(42); // Seed for reproducibility

  printf("Starting Benchmarks...\n");
  printf("Config: NUM_ALLOCS=%d, MAX_SIZE=%d\n\n", NUM_ALLOCS, MAX_SIZE);

  printf("Running Mini-Malloc Benchmark...\n");
  BenchmarkResult mini_res = run_test(true);
  mini_stats();
  mini_cleanup();

  printf("\nRunning Standard Malloc Benchmark...\n");
  BenchmarkResult std_res = run_test(false);

  printf("\n--- Comparison Results ---\n");
  printf("%-20s %-15s %-15s\n", "Metric", "Mini-Malloc", "Std Malloc");
  printf("------------------------------------------------------------\n");
  printf("%-20s %-15.6f %-15.6f\n", "Time (sec)", mini_res.time_taken,
         std_res.time_taken);
  printf("%-20s %-15d %-15d\n", "Total Attempts", mini_res.total_attempts,
         std_res.total_attempts);
  printf("%-20s %-15d %-15d\n", "Success Count", mini_res.success_count,
         std_res.success_count);
  printf("%-20s %-15.2f%% %-15.2f%%\n", "Success Rate",
         (double)mini_res.success_count / mini_res.total_attempts * 100,
         (double)std_res.success_count / std_res.total_attempts * 100);
  printf("%-20s %-15.6f %-15.6f\n", "Avg Latency (us)",
         (mini_res.time_taken / mini_res.total_attempts) * 1e6,
         (std_res.time_taken / std_res.total_attempts) * 1e6);

  return 0;
}
