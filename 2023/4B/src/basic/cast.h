#ifndef CAST_H
#define CAST_H

#include "src/core/tensor.h"

/**
 * @brief Applies a cast over an input signal with specified DataType.
 *
 * @param input The input tensor to be casted.
 * @param dtype The DataType to cast to.
 * @param output The output tensor pointer.
 * @return Status The Status enum indicates whether the routine is OK.
 */
AITISA_API_PUBLIC Status aitisa_cast(const Tensor input, DataType dtype,
                                     Tensor *output);
#endif
