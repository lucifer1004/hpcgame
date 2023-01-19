#ifndef SQUEEZE_H
#define SQUEEZE_H

#include "src/core/tensor.h"

/**
 * @brief Applies a squeeze over an input tensor along specified axises,
 * deleting dimensions of 1.
 *
 * @param input The input tensor.
 * @param axis The axis array to point out which dimension to delete. If
 * axis[i]==0, then do nothing along the ith axis.
 * @param num_axis The length of array axis. If num_axis==0, then deal
 * with all dimensions of 1.
 * @param output The output tensor pointer.
 * @return Status The Status enum indicates whether the routine is OK.
 */
AITISA_API_PUBLIC Status aitisa_squeeze(const Tensor input, int64_t *axis,
                                        int64_t num_axis, Tensor *output);

#endif  // SQUEEZE_H
