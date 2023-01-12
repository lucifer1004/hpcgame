#include "rhs.h"
#include <cmath>
#define PI25DT 3.141592653589793238462643
#define doublePIsquare 19.73920880217871723766898

void rhs(double &x, double &y, double &value) {
  value = doublePIsquare * std::sin(PI25DT * x) * std::sin(PI25DT * y);
}
