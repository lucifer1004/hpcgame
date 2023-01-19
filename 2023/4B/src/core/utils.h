#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

/*
 *  Product of all dimensions
 */
static inline int64_t size_of_dims(int64_t *dims, int ndim) {
  int64_t r = 1;
  for (int64_t i = 0; i < ndim; ++i) {
    r *= dims[i];
  }
  return r;
}

/*
 *  Product of all dimensions starting from k
 */
static inline int64_t size_from_dim(int64_t k, int64_t *dims, int ndim) {
  int64_t r = 1;
  for (int64_t i = k; i < ndim; ++i) {
    r *= dims[i];
  }
  return r;
}

/*
 * Product of all dims up to k (not including dims[k])
 */
static inline int64_t size_to_dim(int64_t k, int64_t *dims, int ndim) {
  int64_t r = 1;
  for (int i = 0; i < k; ++i) {
    r *= dims[i];
  }
  return r;
}

/*
 * Product of all dims between k and l (not including dims[k] and dims[l])
 */
static inline int64_t size_between_dim(int64_t k, int64_t l, int64_t *dims, int ndim) {
  int64_t r = 1;
  if (k < l) {
    for (int i = k + 1; i < l; ++i) {
      r *= dims[i];
    }
  } else {
    for (int i = l + 1; i < k; ++i) {
      r *= dims[i];
    }
  }
  return r;
}

#endif