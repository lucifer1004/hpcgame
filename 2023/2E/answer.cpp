#include "xsimd/xsimd.hpp"
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

using vector_type = std::vector<float, xsimd::aligned_allocator<float, 64>>;
constexpr std::size_t simd_size = xsimd::simd_type<float>::size;

double lut[15][10]{
    {0e-1, 1e-1, 2e-1, 3e-1, 4e-1, 5e-1, 6e-1, 7e-1, 8e-1, 9e-1},
    {0e-2, 1e-2, 2e-2, 3e-2, 4e-2, 5e-2, 6e-2, 7e-2, 8e-2, 9e-2},
    {0e-3, 1e-3, 2e-3, 3e-3, 4e-3, 5e-3, 6e-3, 7e-3, 8e-3, 9e-3},
    {0e-4, 1e-4, 2e-4, 3e-4, 4e-4, 5e-4, 6e-4, 7e-4, 8e-4, 9e-4},
    {0e-5, 1e-5, 2e-5, 3e-5, 4e-5, 5e-5, 6e-5, 7e-5, 8e-5, 9e-5},
    {0e-6, 1e-6, 2e-6, 3e-6, 4e-6, 5e-6, 6e-6, 7e-6, 8e-6, 9e-6},
    {0e-7, 1e-7, 2e-7, 3e-7, 4e-7, 5e-7, 6e-7, 7e-7, 8e-7, 9e-7},
    {0e-8, 1e-8, 2e-8, 3e-8, 4e-8, 5e-8, 6e-8, 7e-8, 8e-8, 9e-8},
    {0e-9, 1e-9, 2e-9, 3e-9, 4e-9, 5e-9, 6e-9, 7e-9, 8e-9, 9e-9},
    {0e-10, 1e-10, 2e-10, 3e-10, 4e-10, 5e-10, 6e-10, 7e-10, 8e-10, 9e-10},
    {0e-11, 1e-11, 2e-11, 3e-11, 4e-11, 5e-11, 6e-11, 7e-11, 8e-11, 9e-11},
    {0e-12, 1e-12, 2e-12, 3e-12, 4e-12, 5e-12, 6e-12, 7e-12, 8e-12, 9e-12},
    {0e-13, 1e-13, 2e-13, 3e-13, 4e-13, 5e-13, 6e-13, 7e-13, 8e-13, 9e-13},
    {0e-14, 1e-14, 2e-14, 3e-14, 4e-14, 5e-14, 6e-14, 7e-14, 8e-14, 9e-14},
    {0e-15, 1e-15, 2e-15, 3e-15, 4e-15, 5e-15, 6e-15, 7e-15, 8e-15, 9e-15},
};

template <typename T> void read(FILE *f, T &x) {
  x = 0;
  char c = getc(f);
  T sig = 1;
  for (; !isdigit(c); c = getc(f))
    if (c == '-')
      sig = -1;
  for (; isdigit(c); c = getc(f))
    x = (x << 3) + (x << 1) + c - '0';
  x *= sig;
}

void read_float(FILE *f, float &v) {
  char c = getc(f);
  std::size_t x = 0, z = 0;
  std::size_t i = 0;
  std::size_t sig = 1, sigz = 1;
  for (; !isdigit(c); c = getc(f))
    if (c == '-')
      sig = -1;
  for (; isdigit(c); c = getc(f)) {
    x = x * 10 + c - '0';
  }
  if (c == '.') {
    for (c = getc(f); isdigit(c); c = getc(f)) {
      if (i < 12) {
        x = x * 10 + c - '0';
        i++;
      }
    }
  }
  if (c == 'e') {
    c = getc(f);
    if (c == '-') {
      sigz = -1;
    }
    for (c = getc(f); isdigit(c); c = getc(f)) {
      z = z * 10 + c - '0';
    }
    z *= sigz;
  }

  v = x * lut[i - 1 - z][1];
}

int main() {
  FILE *ifile = fopen("input.txt", "r");
  std::size_t N, M;
  read(ifile, N);
  read(ifile, M);

  vector_type a(N * M);
  for (std::size_t i = 0; i < N; i++) {
    for (std::size_t j = 0; j < M; j++) {
      read_float(ifile, a[i * M + j]);
      // fscanf(ifile, "%f", &a[i * M + j]);
    }
  }
  fclose(ifile);

  FILE *wfile = fopen("weight.txt", "r");
  std::size_t C, D;
  fscanf(wfile, "%lu %lu", &C, &D);
  vector_type w(C * D);
  for (std::size_t i = 0; i < C; i++) {
    for (std::size_t j = 0; j < D; j++) {
      fscanf(wfile, "%f", &w[i * D + j]);
    }
  }
  fclose(wfile);

  std::size_t P = N - C + 1;
  std::size_t Q = M - D + 1;
  vector_type res(P * Q);

  for (std::size_t i = 0; i < P; i++) {
    for (std::size_t j = 0; j < Q; j++) {
      for (std::size_t k = 0; k < C; k++) {
        for (std::size_t l = 0; l < D; l += simd_size) {
          auto ba = xsimd::load_unaligned(&a[(i + k) * M + j + l]);
          auto bw = xsimd::load_aligned(&w[k * D + l]);
          auto bres = ba * bw;
          res[i * Q + j] += xsimd::reduce_add(bres);
        }
      }
    }
  }

  FILE *ofile = fopen("output.txt", "w");
  fprintf(ofile, "%lu %lu\n", P, Q);
  for (int i = 0; i < P; i++) {
    for (int j = 0; j < Q; j++) {
      fprintf(ofile, "%.6f ", res[i * Q + j]);
    }
    fprintf(ofile, "\n");
  }
  fclose(ofile);

  return 0;
}