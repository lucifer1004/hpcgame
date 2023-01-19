#ifndef FACTORIES_H
#define FACTORIES_H

#include "src/core/tensor.h"

/**
 * @brief Full a tensor with specific value.
 *
 * @param dtype DataType of the tensor
 * @param device Device of the tensor
 * @param dims Dimension array of the tensor
 * @param ndim Dimension size of the tensor
 * @param value Value to be set
 * @param output Tensor to be fulled
 * @return Status
 */
AITISA_API_PUBLIC Status aitisa_full(DataType dtype, Device device,
                                     int64_t *dims, int64_t ndim, double value,
                                     Tensor *output);
#endif
