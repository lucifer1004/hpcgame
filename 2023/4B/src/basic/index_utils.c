#include <assert.h>
#include "src/basic/index_utils.h"

int64_t aitisa_get_stride(const Tensor t, int64_t dimension) {
  int64_t ndim = aitisa_tensor_ndim(t);
  int64_t *dims = aitisa_tensor_shape(t).dims;
  int64_t *min2maj = aitisa_tensor_shape(t).layout.min2maj;
  int64_t stride = 1;
  for (int64_t d = 0; d < ndim; ++d) {
    int64_t dim = min2maj[d];
    if (dim == dimension) {
      break;
    }
    stride *= dims[dim];
  }
  return stride;
}

void aitisa_get_all_strides(const Tensor t, int64_t *strides) {
  int64_t ndim = aitisa_tensor_ndim(t);
  int64_t *dims = aitisa_tensor_shape(t).dims;
  int64_t *min2maj = aitisa_tensor_shape(t).layout.min2maj;
  int64_t stride = 1;
  for (int d = 0; d < ndim; ++d) {
    int64_t dim = min2maj[d];
    strides[dim] = stride;
    stride *= dims[dim];
  }
}

int64_t aitisa_coords_to_linidx(const Tensor t, int64_t *coords, int64_t len) {
  int64_t ndim = aitisa_tensor_ndim(t);
  int64_t *dims = aitisa_tensor_shape(t).dims;

  assert(len == ndim);
  for (size_t i = 0; i < ndim; ++i) {
    assert(coords[i] >= 0);
    assert(coords[i] < dims[i]);
  }

  int64_t scale = 1;
  int64_t linidx = 0;
  for (int64_t d = ndim - 1; d >= 0; --d) {
    linidx += scale * coords[d];
    scale *= dims[d];
  }
  return linidx;
}

void aitisa_linidx_to_coords(const Tensor t, int64_t linidx, int64_t *coords) {
  int64_t size = aitisa_tensor_size(t);
  int64_t ndim = aitisa_tensor_ndim(t);
  int64_t *dims = aitisa_tensor_shape(t).dims;

  assert(linidx >= 0);
  assert(linidx < size);

  int64_t divisor = 1;
  for (int64_t d = ndim - 1; d >= 0; --d) {
    coords[d] = (linidx / divisor) % dims[d];
    divisor *= dims[d];
  }
}

int64_t aitisa_coords_to_offset(const Tensor t, int64_t *coords, int64_t len) {
  int64_t ndim = aitisa_tensor_ndim(t);
  int64_t *dims = aitisa_tensor_shape(t).dims;
  int64_t *min2maj = aitisa_tensor_shape(t).layout.min2maj;

  assert(len == ndim);
  for (size_t i = 0; i < ndim; ++i) {
    assert(coords[i] >= 0);
    assert(coords[i] < dims[i]);
  }

  int64_t scale = 1;
  int64_t offset = 0;
  int first = 1;
  for (int64_t d = 0; d < ndim; ++d) {
    int64_t dim = min2maj[d];
    if (first) {
      // Avoid two multiplies on the first loop iteration
      offset = coords[dim];
      scale = dims[dim];
      first = 0;
    } else {
      offset += scale * coords[dim];
      scale *= dims[dim];
    }
  }
  return offset;
}

void aitisa_offset_to_coords(const Tensor t, int64_t offset, int64_t *coords) {
  int64_t size = aitisa_tensor_size(t);
  int64_t ndim = aitisa_tensor_ndim(t);
  int64_t *dims = aitisa_tensor_shape(t).dims;
  int64_t *min2maj = aitisa_tensor_shape(t).layout.min2maj;

  assert(offset >= 0);
  assert(offset < size);

  int64_t divisor = 1;
  for (int64_t d = 0; d < ndim; ++d) {
    int64_t dim = min2maj[d];
    coords[dim] = (offset / divisor) % dims[dim];
    divisor *= dims[dim];
  }
}
