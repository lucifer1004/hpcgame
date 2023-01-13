#include "xsimd/xsimd.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

using vector_type = std::vector<float, xsimd::aligned_allocator<float, 64>>;

int main() {
  // the code do not make any sense, just to make sure xsimd works
  std::ifstream ifile, wfile;
  ifile.open("input.txt");
  std::size_t N, M;
  ifile >> N >> M;
  vector_type a(N * M);
  for (std::size_t i = 0; i < N; i++) {
    for (std::size_t j = 0; j < M; j++) {
      ifile >> a[i * M + j];
    }
  }
  ifile.close();

  wfile.open("weight.txt");
  std::size_t C, D;
  wfile >> C >> D;
  vector_type w(C * D);
  for (std::size_t i = 0; i < C; i++) {
    for (std::size_t j = 0; j < D; j++) {
      wfile >> w[i * D + j];
    }
  }
  wfile.close();

  std::size_t P = N - C + 1;
  std::size_t Q = M - D + 1;
  vector_type res(P * Q);

  constexpr std::size_t simd_size = xsimd::simd_type<float>::size;

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

  std::ofstream ofile;
  ofile.open("output.txt");
  ofile << P << " " << Q << std::endl;
  for (int i = 0; i < P; i++) {
    for (int j = 0; j < Q; j++) {
      ofile << res[i * Q + j] << std::setprecision(12) << " ";
    }
    ofile << std::endl;
  }
  ofile.close();

  return 0;
}