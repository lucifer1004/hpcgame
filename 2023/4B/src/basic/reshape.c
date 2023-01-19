#include "src/basic/reshape.h"
#include "src/core/utils.h"

Status aitisa_reshape(const Tensor input, int64_t* dims, int64_t ndim,
                      Tensor* output) {
  // Check if the sizes of input and output are matched
  int64_t in_size = aitisa_tensor_size(input);
  int64_t out_size = size_of_dims(dims, ndim);
  if (in_size != out_size) {
    return STATUS_DIMENSIONS_MISMATCH;
  }
  // Create output
  Device device = aitisa_tensor_device(input);
  // LayoutType layout_type = aitisa_tensor_layout_type(input);
  DataType dtype = aitisa_tensor_data_type(input);
  Tensor new_tensor;
  CHECK_STATUS(
      aitisa_create(dtype, device, dims, ndim, NULL, 0, &new_tensor));
  *output = new_tensor;
  // Transport data
  void* in_data = aitisa_tensor_data(input);
  void* out_data = aitisa_tensor_data(*output);
  int64_t ele_size = dtype.size;
  memcpy(out_data, in_data, out_size * ele_size);

  return STATUS_SUCCESS;
}
