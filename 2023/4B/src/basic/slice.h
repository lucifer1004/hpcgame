#ifndef SLICE_H
#define SLICE_H

#include "src/core/tensor.h"

/**
 * @brief Applies a slice over an input tensor within a specified range.
 *
 * @param input The input tensor.
 * @param begin The beginning site of each dimension.
 * @param size The size of slice range.
 * @param step The slice step of each dimension.
 * @param output The output tensor pointer.
 * @return Status The Status enum indicates whether the routine is OK.
 */
AITISA_API_PUBLIC Status aitisa_slice(const Tensor input, int *begin,
                                      int *size, int *step,
                                      Tensor *output);
#endif
