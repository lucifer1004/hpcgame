#include "src/basic/slice.h"
#include <inttypes.h>
#include <stdbool.h>
#include <math.h>
#include "src/core/allocator.h"
#include "src/core/tensor.h"
#include "src/core/dispatch.h"

static Status slice_create_output(const Tensor input, int *begin, int *size,
                                  int *step, Tensor *output) {
  int64_t in_ndim = aitisa_tensor_ndim(input);
  // Calculate the dimensions of output
  int *out_dims_temp = aitisa_default_cpu_allocator()->raw_alloc(
      sizeof(*out_dims_temp) * in_ndim);
  if (!out_dims_temp) {
    return STATUS_ALLOC_FAILED;
  }
  int64_t out_ndim = 0;
  for (int i = 0; i < in_ndim; i++) {
    out_dims_temp[i] = (int)ceil((double)size[i] / (double)step[i]);
    if (out_dims_temp[i] > 0) {
      out_ndim++;
    }
  }
  int64_t *out_dims =
      aitisa_default_cpu_allocator()->raw_alloc(sizeof(*out_dims) * out_ndim);
  int last_idx = 0;
  for (int i = 0; i < in_ndim; i++) {
    if (out_dims_temp[i] > 0) {
      out_dims[last_idx++] = out_dims_temp[i];
    }
  }
  aitisa_default_cpu_allocator()->raw_dealloc(out_dims_temp);
  out_dims_temp = NULL;
  // Create output
  Status status;
  Tensor new_tensor;
  DataType dtype = aitisa_tensor_data_type(input);
  Device device = aitisa_tensor_device(input);
  // LayoutType layout_type = aitisa_tensor_layout_type(input);
  status = aitisa_create(dtype, device, out_dims, out_ndim,
                         NULL, 0, &new_tensor);
  *output = new_tensor;
  aitisa_default_cpu_allocator()->raw_dealloc(out_dims);
  out_dims = NULL;
  
  return status;
}

static Status slice_check_parameters(const Tensor input, int *begin, int *size,
                                     int *step) {
  int64_t in_ndim = aitisa_tensor_ndim(input);
  int64_t *in_dims = aitisa_tensor_dims(input);
  for (int i = 0; i < in_ndim; i++) {
    bool cond_begin = begin[i] < 0 || begin[i] >= in_dims[i];
    bool cond_size  = size[i] < 0  || begin[i] + size[i] > in_dims[i];
    bool cond_step  = step[i] < 1;
    if (cond_begin || cond_size || cond_step) {
      return STATUS_INVALID_ARGUMENT;
    }
  }
  return STATUS_SUCCESS;
}

#define slice_kernel(typename)                                                \
  typename *in_data = (typename *)aitisa_tensor_data(input);                  \
  typename *out_data = (typename *)aitisa_tensor_data(*output);               \
  int64_t out_size = aitisa_tensor_size(*output);                             \
  for (int out_linear_idx = 0; out_linear_idx < out_size; out_linear_idx++) { \
    /* Get the linear index of input data element */                          \
    int64_t in_linear_idx = 0;                                                \
    for (int i = 0; i < in_ndim; i++) {                                       \
      in_linear_idx += index_recorder[i] * offset_recorder[i];                \
    }                                                                         \
    /* Transport the input data element to output data */                     \
    out_data[out_linear_idx] = in_data[in_linear_idx];                        \
    /* Update the index_recorder */                                           \
    for (int i = in_ndim - 1; i >= 0; i--) {                                  \
      index_recorder[i] += step[i];                                           \
      /* Judge whether the index is out of boundary */                        \
      if (index_recorder[i] > boundary[i]) {                                  \
        index_recorder[i] = begin[i];                                         \
      } else {                                                                \
        break;                                                                \
      }                                                                       \
    }                                                                         \
  }

static Status slice_template(const Tensor input, int *begin, int *size,
                             int *step, Tensor *output) {
  int64_t in_ndim = aitisa_tensor_ndim(input);
  int64_t *in_dims = aitisa_tensor_dims(input);
  // Make an index_recorder which records the index of present input
  // element being processed, then initialize it
  int64_t *index_recorder = aitisa_default_cpu_allocator()->raw_alloc(
      sizeof(*index_recorder) * in_ndim);
  // Make a boundary to judge whether the index is out of slice range
  int64_t *boundary =
      aitisa_default_cpu_allocator()->raw_alloc(sizeof(*boundary) * in_ndim);
  // Make an offset_recorder which records every linear offset of each
  // dimension
  int64_t *offset_recorder = aitisa_default_cpu_allocator()->raw_alloc(
      sizeof(*offset_recorder) * in_ndim);
  if (!index_recorder || !boundary || !offset_recorder) {
    return STATUS_ALLOC_FAILED;
  }
  for (int i = 0; i < in_ndim; i++) {
    index_recorder[i] = begin[i];
    boundary[i] = (int64_t)begin[i] + (int64_t)size[i] - 1;
  }
  offset_recorder[in_ndim - 1] = 1;
  for (int i = in_ndim - 2; i >= 0; i--) {
    offset_recorder[i] = offset_recorder[i + 1] * in_dims[i + 1];
  }
  // Implement slice kernel
  DataType dtype = aitisa_tensor_data_type(input);
  AITISA_DISPATCH_ALL_TYPES_RETURN(dtype, slice_kernel);

  aitisa_default_cpu_allocator()->raw_dealloc(index_recorder);
  aitisa_default_cpu_allocator()->raw_dealloc(boundary);
  aitisa_default_cpu_allocator()->raw_dealloc(offset_recorder);
  return STATUS_SUCCESS;
}

Status aitisa_slice(const Tensor input, int *begin, int *size, int *step,
                    Tensor *output) {
  // Make sure parameters are correct
  CHECK_STATUS(slice_check_parameters(input, begin, size, step));
  // Create output
  CHECK_STATUS(slice_create_output(input, begin, size, step, output));
  // Implement slice
  Status status;
  status = slice_template(input, begin, size, step, output);
  return status;
}
