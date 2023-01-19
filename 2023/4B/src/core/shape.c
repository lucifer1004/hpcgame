#include "src/core/shape.h"
#include "src/core/status.h"
#include "src/core/allocator.h"

Status aitisa_create_default_layout(LayoutType layout_type, int64_t ndim,
                                    Layout *layout) {
  if (layout_type != LAYOUT_DENSE) return STATUS_NOT_SUPPORTED;
  layout->type = layout_type;
  if(ndim == 0){
    layout->min2maj = NULL;
  }else{
    layout->min2maj = aitisa_default_cpu_allocator()->raw_alloc(
      sizeof(*layout->min2maj) * ndim);
    if (!layout->min2maj) return STATUS_ALLOC_FAILED;
  }
  for (int64_t i = 0; i < ndim; ++i) {
    layout->min2maj[i] = ndim - 1 - i;
  }
  return STATUS_SUCCESS;
}

Status aitisa_destroy_layout(Layout *layout){
  if (!layout) return STATUS_SUCCESS;
  if(layout->min2maj){
    aitisa_default_cpu_allocator()->raw_dealloc(layout->min2maj);
    layout->min2maj = NULL;
  }
  return STATUS_SUCCESS;
}

Status aitisa_create_shape(LayoutType layout_type, int64_t *dims, int64_t ndim,
                           Shape *shape) {
  shape->ndim = ndim;
  if(ndim == 0){
    shape->dims = NULL;
  }else{
    shape->dims =
      aitisa_default_cpu_allocator()->raw_alloc(sizeof(*shape->dims) * ndim);
    if (!shape->dims) return STATUS_ALLOC_FAILED;
  }
  for (int64_t i = 0; i < ndim; ++i) {
    shape->dims[i] = dims[i];
  }
  CHECK_STATUS(aitisa_create_default_layout(layout_type, ndim, &shape->layout));
  return STATUS_SUCCESS;
}

Status aitisa_destroy_shape(Shape *shape){
  if (!shape) return STATUS_SUCCESS;
  if(shape->dims){
    aitisa_default_cpu_allocator()->raw_dealloc(shape->dims);
    shape->dims = NULL;
  }
  CHECK_STATUS(aitisa_destroy_layout(&(shape->layout)));
  return STATUS_SUCCESS;
}
