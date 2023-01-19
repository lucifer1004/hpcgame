#include "src/basic/duplicate.h"
#include "src/basic/slice.h"
#include "src/core/allocator.h"

Status aitisa_duplicate(const Tensor input, Tensor *output) {
  Tensor new_tensor;
  int64_t *dims = aitisa_tensor_dims(input);
  int ndim = (int)(aitisa_tensor_ndim(input));
  int *begin = aitisa_default_cpu_allocator()->raw_alloc(sizeof(*begin) * ndim);
  int *size = aitisa_default_cpu_allocator()->raw_alloc(sizeof(*size) * ndim);
  int *step = aitisa_default_cpu_allocator()->raw_alloc(sizeof(*step) * ndim);
  if (!begin || !size || !step) {
    return STATUS_ALLOC_FAILED;
  }
  for (int64_t i = 0; i < ndim; i++) {
    begin[i] = 0;
    size[i] = (int)(dims[i]);
    step[i] = 1;
  }
  Status status = aitisa_slice(input, begin, size, step, &new_tensor);
  if (status == STATUS_SUCCESS) {
    *output = new_tensor;
  }
  aitisa_default_cpu_allocator()->raw_dealloc(begin);
  aitisa_default_cpu_allocator()->raw_dealloc(size);
  aitisa_default_cpu_allocator()->raw_dealloc(step);
  return status;
}
