#ifndef MATMUL_SIMPLE_H
#define MATMUL_SIMPLE_H

#include "src/core/tensor.h"

/**
 * @brief Matrix multiplication of tensor1 and tensor2.
 *
 * @details Matrix multiplication of tensor1 and tensor2.
 * @param tensor1 The first input.
 * @param tensor2 The second input.
 * @param output Pointer to the result tensor, the output will be resized inside the function.
 *
 * @return Status.
 * @retval STATUS_SUCCESS Success.
 * @retval STATUS_NOT_SUPPORTED The device type or data type is not supported.
 * @retval STATUS_INVALID_ARGUMENT The dims of inputs are invalid.
 * @retval STATUS_DIMENSIONS_MISMATCH The dims of inputs are mismatched.
 */
AITISA_API_PUBLIC Status aitisa_matmul_simple(const Tensor tensor1,
                                              const Tensor tensor2, Tensor *output);

#endif
