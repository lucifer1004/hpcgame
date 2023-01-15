#include <malloc.h>
#include <omp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int cmp(const void *a, const void *b) {
  uint64_t x = *(uint64_t *)a;
  uint64_t y = *(uint64_t *)b;
  if (x < y) {
    return -1;
  } else if (x > y) {
    return 1;
  } else {
    return 0;
  }
}

int main() {
  uint64_t gen1, gen2, gen3, n, ans;
  scanf("%lu %lu %lu %lu", &gen1, &gen2, &gen3, &n);
  uint64_t *arr = (uint64_t *)malloc(sizeof(uint64_t) * n);
  for (uint64_t i = 0; i < n; ++i) {
    gen1 ^= gen1 << 31;
    gen1 ^= gen1 >> 17;
    gen2 ^= gen2 << 13;
    gen2 ^= gen2 >> 5;
    gen3++;
    gen1 ^= gen2;
    gen2 ^= gen3;
    arr[i] = gen1;
  }
  qsort(arr, n, sizeof(uint64_t), cmp);

  omp_set_num_threads(omp_get_num_threads());
#pragma omp parallel for reduction(^ : ans)
  for (uint64_t i = 0; i < n; ++i) {
    ans ^= i * arr[i];
  }

  printf("%lu\n", ans);
  return 0;
}