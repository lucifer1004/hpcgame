#include "src/core/tensor.h"
#include "src/core/allocator.h"
#include "src/core/utils.h"
// #include <stdio.h>

Status aitisa_create(DataType dtype, Device device, int64_t *dims, int64_t ndim, 
                     void *data, unsigned int len, Tensor *output) {
  Tensor tensor;
  tensor = aitisa_default_cpu_allocator()->raw_alloc(sizeof(*tensor));
  if (!tensor) return STATUS_ALLOC_FAILED;
  tensor->size = size_of_dims(dims, ndim);
  tensor->offset = 0;
  if(data){
    if(len/dtype.size != tensor->size) return STATUS_INVALID_ARGUMENT;
  }
  CHECK_STATUS(aitisa_create_shape(LAYOUT_DENSE, dims, ndim, &tensor->shape));
  CHECK_STATUS(
      aitisa_create_storage(dtype, device, tensor->size, data, &tensor->storage));
  *output = tensor;
  return STATUS_SUCCESS;
}

Status aitisa_destroy(Tensor *input) {
  if (!(*input)) return STATUS_SUCCESS;
  CHECK_STATUS(aitisa_destroy_shape(&(*input)->shape));
  CHECK_STATUS(aitisa_destroy_storage(&((*input)->storage)));
  aitisa_default_cpu_allocator()->raw_dealloc((*input));
  *input = NULL;
  return STATUS_SUCCESS;
}

Status aitisa_resolve(Tensor input, DataType *dtype, Device *device, 
                      int64_t **dims, int64_t *ndim, void **data, unsigned int *len){
  *dtype = aitisa_tensor_data_type(input);
  *device = aitisa_tensor_device(input);
  int64_t *dims_ = aitisa_tensor_dims(input);
  *dims = dims_;
  *ndim = aitisa_tensor_ndim(input);
  void *data_ = aitisa_tensor_data(input);
  *data = data_;
  *len = aitisa_tensor_size(input) * (*dtype).size;
}
