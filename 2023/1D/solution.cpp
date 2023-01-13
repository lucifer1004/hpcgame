#include "omp.h"
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>

const char *INFILE = "input.bin";
const char *OUTFILE = "output.bin";

int main() {
  struct stat st;
  stat(INFILE, &st);
  int size = st.st_size;

  int i_file = open(INFILE, O_RDONLY, 0);
  int *i_mem = reinterpret_cast<int *>(
      mmap(NULL, size, PROT_READ, MAP_SHARED, i_file, 0));
  int P = i_mem[0];
  omp_set_num_threads(P);
  int N = i_mem[1];
  assert(P > 1 && P <= 16);
  assert(size == (N + 2) * sizeof(int));

  int o_file =
      open(OUTFILE, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
  posix_fallocate(o_file, 0, (N + 1) * sizeof(int));
  int *o_mem = reinterpret_cast<int *>(
      mmap(NULL, (N + 1) * sizeof(int), PROT_WRITE, MAP_SHARED, o_file, 0));

  unsigned int i;
  long long sum = 0;

#pragma omp parallel for reduction(+ : sum) shared(i_mem, o_mem) private(i)
  for (i = 0; i < N; i++) {
    o_mem[i + 1] = i_mem[i + 2] + 1;
    sum += i_mem[i + 2];
  }

  o_mem[0] = sum % 100001651;
  if (o_mem[0] < 0) {
    o_mem[0] += 100001651;
  }
  std::cout << sum << " " << o_mem[0] << std::endl;
  return 0;
}