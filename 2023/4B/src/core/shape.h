#ifndef SHAPE_H
#define SHAPE_H

#include <stdint.h>
#include <stdlib.h>

#include "src/core/macros.h"
#include "src/core/status.h"

/**
 * @brief Enumeration type for all possible layout types
 *
 * @details Use to select the right layout types for tensor
 */
typedef enum {
  LAYOUT_DENSE = 0, /**< Represent for dense tensor */
  LAYOUT_SPARSE = 1, /**< Represent for sparse tensor*/
} LayoutType;

/**
 * @brief Attributes of Layout struct
 *
 * @details Layout structure contains all attributes which need to be known in a specific tensor
 */
typedef struct {
  LayoutType type; /**< The layout type, include sparse and dense */
  int64_t *min2maj; /**< The layout message, which indicate the visit order of tensor */
} Layout;

/**
 * @brief Attributes of Shape struct
 *
 * @details Shape structure contains all attributes which need to be known in a specific tensor
 */
typedef struct {
  int64_t ndim; /**< Number of dimensions */
  int64_t *dims; /**< Number of elements in each dimension*/
  Layout layout; /**< The layout information of a tensor */
} Shape;

/**
 * @brief Create a new shape using the specific parameters
 *
 * @param layout_type The layout type of shape
 * @param dims The dimension detail of this shape
 * @param ndim Number of dimension of this shape
 * @param shape A new shape to be created
 *
 * @code
 * Shape shape;
 * int64_t dims[3] = {2, 3, 4};
 * aitisa_create_shape(LAYOUT_DENSE, dims, 3, shape);
 * @endcode
 *
 * @return
 * @retval STATUS_SUCCESS Successfully create a new Shape
 * @retval STATUS_ALLOC_FAILED Failed when the shape already exists
 */
Status aitisa_create_shape(LayoutType layout_type, int64_t *dims,
                           int64_t ndim, Shape *shape);

/**
 * @brief Destroy an exist shape
 *
 * @param shape the shape to be destroy
 *
 * @return
 * @retval STATUS_SUCCESS Successfully destroy a shape
 */
Status aitisa_destroy_shape(Shape *shape);

#endif
