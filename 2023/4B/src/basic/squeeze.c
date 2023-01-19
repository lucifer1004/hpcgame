#include "src/basic/squeeze.h"
#include "src/basic/reshape.h"
#include "src/core/allocator.h"

Status aitisa_squeeze(const Tensor input, int64_t *axis, int64_t num_axis,
                      Tensor *output) {
  int64_t *in_dims = aitisa_tensor_dims(input);
  int64_t in_ndim = aitisa_tensor_ndim(input);
  // Check if axis and num_axis are valid
  if (num_axis < 0) return STATUS_INVALID_ARGUMENT;
  if (num_axis > 0) {
    for (int64_t i = 0; i < num_axis; i++) {
      if (axis[i] < 0) {
        return STATUS_INVALID_ARGUMENT;
      }
    }
  }
  // Make the intermediate dimensions
  int64_t *processed_in_dims = aitisa_default_cpu_allocator()->raw_alloc(
      sizeof(*processed_in_dims) * in_ndim);
  if (!processed_in_dims) return STATUS_ALLOC_FAILED;
  for (int64_t i = 0; i < in_ndim; i++) {
    processed_in_dims[i] = in_dims[i];
  }
  // If num_axis == 0, then process all one-dimension axises
  int64_t out_ndim = aitisa_tensor_ndim(input);
  if (num_axis == 0) {
    for (int64_t i = 0; i < in_ndim; i++) {
      if (in_dims[i] == 1) {
        processed_in_dims[i] = 0;
        out_ndim--;
      }
    }
  } else if (num_axis > 0) {
    for (int64_t i = 0; i < num_axis; i++) {
      if (in_dims[axis[i]] == 1) {
        processed_in_dims[axis[i]] = 0;
        out_ndim--;
      }
    }
  }

  // Create dims and ndim of output
  int64_t *out_dims =
      aitisa_default_cpu_allocator()->raw_alloc(sizeof(*out_dims) * out_ndim);
  if (!out_dims) return STATUS_ALLOC_FAILED;
  int64_t j = 0;
  for (int64_t i = 0; i < in_ndim; i++) {
    if (processed_in_dims[i] != 0) {
      out_dims[j++] = processed_in_dims[i];
    }
  }
  // Use reshape
  Status status = aitisa_reshape(input, out_dims, out_ndim, output);
  // Destroy temporary parameters
  aitisa_default_cpu_allocator()->raw_dealloc(processed_in_dims);
  aitisa_default_cpu_allocator()->raw_dealloc(out_dims);
  processed_in_dims = NULL;
  out_dims = NULL;
  return status;
}
