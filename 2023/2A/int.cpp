#include "omp.h"
#include "rhs.h"
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>

int main(int argc, char *argv[]) {
  assert(argc >= 2);
  int N = atoi(argv[1]);
  omp_set_num_threads(8);

  double sum = 0.0;
#pragma omp parallel for reduction(+ : sum)
  for (int i = 0; i < N; i++) {
    double xi = ((double)i + 0.5) / N;
    double yi;
    rhs(xi, yi);
    sum = sum + yi;
  }

  std::ofstream myfile;
  myfile.open("output.dat");
  myfile << std::setprecision(30) << sum / N << std::endl;
  myfile.close();
  return 0;
}