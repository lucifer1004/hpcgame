#ifndef DUPLICATE_H
#define DUPLICATE_H

#include "src/core/tensor.h"

/**
 * @brief Applies a duplicate over an input signal.
 *
 * @param input The input tensor to be duplicated.
 * @param output The output tensor pointer.
 * @return Status The Status enum indicates whether the routine is OK.
 */
AITISA_API_PUBLIC Status aitisa_duplicate(const Tensor input,
                                          Tensor *output);
#endif
