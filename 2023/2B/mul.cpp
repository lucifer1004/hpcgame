#include "omp.h"
#include "rhs.h"
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
  assert(argc >= 4);
  unsigned int N1 = atoi(argv[1]);
  unsigned int N2 = atoi(argv[2]);
  unsigned int N3 = atoi(argv[3]);
  std::vector<std::vector<double>> A(N1, std::vector<double>(N2));
  std::vector<std::vector<double>> B(N2, std::vector<double>(N3));
  std::vector<std::vector<double>> C(N1, std::vector<double>(N3));
  omp_set_num_threads(omp_get_num_procs());

  unsigned int t, i, j, k;

#pragma omp parallel for shared(A) private(t)
  for (t = 0; t < N1 * N2; t++) {
    unsigned int i = t / N2, j = t % N2;
    matA(i, j, A[i][j]);
  }

#pragma omp parallel for shared(B) private(t)
  for (t = 0; t < N2 * N3; t++) {
    unsigned int i = t / N3, j = t % N3;
    matB(i, j, B[i][j]);
  }

  if (N1 > N3) {
#pragma omp parallel for shared(A, B, C) private(i, j, k)
    for (i = 0; i < N1; i++) {
      for (j = 0; j < N3; j++) {
        for (k = 0; k < N2; k++) {
          C[i][j] += A[i][k] * B[k][j];
        }
      }
    }
  } else {
#pragma omp parallel for shared(A, B, C) private(i, j, k)
    for (j = 0; j < N3; j++) {
      for (i = 0; i < N1; i++) {
        for (k = 0; k < N2; k++) {
          C[i][j] += A[i][k] * B[k][j];
        }
      }
    }
  }

  std::ofstream myfile;
  myfile.open("output.dat");
  for (i = 0; i < N1; i++) {
    for (j = 0; j < N3; j++) {
      myfile << std::setprecision(12) << C[i][j] << std::endl;
    }
  }
  myfile.close();
  return 0;
}