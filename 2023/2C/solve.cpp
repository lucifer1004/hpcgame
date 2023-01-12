#include "omp.h"
#include "rhs.h"
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
  assert(argc >= 2);
  unsigned int N = atoi(argv[1]);
  omp_set_num_threads(omp_get_num_procs());

  std::vector<std::vector<double>> U(N + 1, std::vector<double>(N + 1));
  std::vector<std::vector<double>> U2(N + 1, std::vector<double>(N + 1));
  std::vector<std::vector<double>> F(N + 1, std::vector<double>(N + 1));

  unsigned i, j;

#pragma omp parallel for shared(F) private(i, j)
  for (i = 1; i < N; ++i) {
    for (j = 1; j < N; ++j) {
      double x = i * 1.0 / N;
      double y = j * 1.0 / N;
      rhs(x, y, F[i][j]);
      F[i][j] /= N * N;
    }
  }

  bool converged[1];
  converged[0] = false;
  while (!converged[0]) {
    converged[0] = true;

#pragma omp parallel for shared(U, U2, converged) private(i, j)
    for (i = 1; i < N; ++i) {
      for (j = 1; j < N; ++j) {
        U2[i][j] =
            (U[i - 1][j] + U[i + 1][j] + U[i][j - 1] + U[i][j + 1] + F[i][j]) /
            4;
        double delta = std::abs(U2[i][j] - U[i][j]);
        if (delta > 1.0e-15) {
          converged[0] = false;
        }
      }
    }

    std::swap(U, U2);
  }

  std::ofstream myfile;
  myfile.open("output.dat");
  for (i = 0; i <= N; ++i) {
    for (j = 0; j <= N; ++j) {
      myfile << std::setprecision(12) << U[i][j] << std::endl;
    }
  }
  myfile.close();
  return 0;
}