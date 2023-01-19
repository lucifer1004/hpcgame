#include "src/core/storage.h"
#include "src/core/allocator.h"

Status aitisa_create_storage(DataType dtype, Device device, int64_t size,
                             void *data, Storage *storage) {
  Storage new_storage;
  new_storage =
      aitisa_default_cpu_allocator()->raw_alloc(
          sizeof(*new_storage));
  if (!new_storage) return STATUS_ALLOC_FAILED;
  new_storage->dtype = dtype;
  new_storage->device = device;
  new_storage->size = size;
  if(data){
    new_storage->data = data;
  }else{
    new_storage->data =
      aitisa_default_cpu_allocator()->raw_alloc(
          dtype.size * size);
    if (!new_storage->data) return STATUS_ALLOC_FAILED;
  }
  *storage = new_storage;
  return STATUS_SUCCESS;
}

Status aitisa_destroy_storage(Storage *storage) {
  if (!(*storage)) return STATUS_SUCCESS;
  void *data = (*storage)->data;
  if(data){
    aitisa_default_cpu_allocator()->raw_dealloc((data));
    data = NULL;
    (*storage)->data = NULL;
  }
  aitisa_default_cpu_allocator()->raw_dealloc((*storage));
  *storage = NULL;
  return STATUS_SUCCESS;
}
