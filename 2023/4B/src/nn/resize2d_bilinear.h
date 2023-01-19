#ifndef RESIZE2D_BILINEAR_H
#define RESIZE2D_BILINEAR_H

#include "src/core/tensor.h"

/**
 * @brief Resize the 2d-image to a given size using Bilinear Interpolation.
 *
 * @param input The input tensor.
 * @param target_h The height of the output image.
 * @param target_w The width of the output image.
 * @param output The output tensor pointer.
 * @return Status The Status enum indicates whether the routine is OK.
 */
AITISA_API_PUBLIC Status aitisa_resize2d_bilinear(const Tensor input, int target_h, int target_w, Tensor *output);

#endif // RESIZE2D_BILINEAR_H
