#ifndef INDEX_UTILS_H
#define INDEX_UTILS_H

#include <stdint.h>

#include "src/core/tensor.h"

/**
 * @brief Get the stride of specific dimension.
 *
 * @param t Tensor
 * @param dimension The specific dimension
 * @return int64_t stride
 */
AITISA_API_PUBLIC int64_t aitisa_get_stride(const Tensor t, int64_t dimension);

/**
 * @brief Get the strides of all dimensions.
 *
 * @param t Tensor
 * @param strides Array to keep the strides
 */
AITISA_API_PUBLIC void aitisa_get_all_strides(const Tensor t, int64_t *strides);

/**
 * @brief Convert the coordinates into linear index.
 *
 * @param t Tensor
 * @param coords Coordinates array
 * @param len Length of coordinates
 * @return int64_t Linear index
 */
AITISA_API_PUBLIC int64_t aitisa_coords_to_linidx(const Tensor t,
                                                  int64_t *coords, int64_t len);

/**
 * @brief Convert the coordinates into linear index.
 *
 * @param t Tensor
 * @param linidx Linear index
 * @param coords Array to keep coordinates
 */
AITISA_API_PUBLIC void aitisa_linidx_to_coords(const Tensor t, int64_t linidx,
                                               int64_t *coords);

/**
 * @brief Convert the coordinates into offset of the underlying physical memory.
 *
 * @param t Tensor
 * @param coords Coordinates array
 * @param len Length of coordinates
 * @return int64_t offset
 */
AITISA_API_PUBLIC int64_t aitisa_coords_to_offset(const Tensor t,
                                                  int64_t *coords, int64_t len);

/**
 * @brief Convert the offset of the underlying physical memory into coordinates.
 *
 * @param t Tensor
 * @param offset Offset of the underlying physical memory
 * @param coords Array to keep coordinates
 */
AITISA_API_PUBLIC void aitisa_offset_to_coords(const Tensor t, int64_t offset,
                                               int64_t *coords);

#endif
