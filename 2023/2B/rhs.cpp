#include "rhs.h"
#include <cmath>

void matA(unsigned int &i, unsigned int &j, double &value) {
  value = std::sin(i + 2 * j);
}

void matB(unsigned int &i, unsigned int &j, double &value) {
  value = std::cos(3 * i + j);
}