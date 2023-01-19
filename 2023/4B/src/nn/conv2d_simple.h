#ifndef CONV2D_SIMPLE_H
#define CONV2D_SIMPLE_H

#include "src/core/tensor.h"

/**
 * @brief Applies a convolution over an input image with a filter.
 *
 * @param input The input tensor of a shape [num_batches, in_channels,
 *              height, width].
 * @param filter The filter tensor of a shape [out_channnels, in_channels,
 *               kernel_size, kernel_size].
 * @param output The output tensor pointer of a shape [num_batches,
 *               out_channels, out_height, out_width].
 * @return Status The Status enum indicates whether the routine is OK.
 */

AITISA_API_PUBLIC Status aitisa_conv2d_simple(const Tensor input, const Tensor filter,
                                              Tensor *output);

#endif
