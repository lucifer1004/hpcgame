#ifndef TENSOR_H
#define TENSOR_H

#include <stdio.h>
#include <string.h>
#include "src/core/shape.h"
#include "src/core/storage.h"
#include "src/core/status.h"
#include "src/core/macros.h"
#include "src/core/types.h"

/**  @brief The implementation for Tensor */
struct _TensorImpl {
  /**< The total number of the elements in the tensor */
  int64_t size;
  /**< the offset of elements to the beginning pointer to data */
  int64_t offset;
  /**<
   * The shape of the tensor includes the dimensions and the layout information.
   */
  Shape shape;
  /**<
   * The Storage of the tensor includes the data type, the device, the size, and
   * the raw data pointer.
   */
  Storage storage;
};

/**
 * @brief The tensor handle for users without exposing the implementation
 *        details.
 */
typedef struct _TensorImpl* Tensor;

/**
 * @brief Get the number of dimensions of the tensor.
 */
static inline int64_t aitisa_tensor_ndim(const Tensor t) {
  return t->shape.ndim;
}

/**
 * @brief Get the size in the specific demension of the tensor.
 */
static inline int64_t aitisa_tensor_dim(const Tensor t, int64_t d) {
  return t->shape.dims[d];
}

/**
 * @brief Get the dimensions of the tensor.
 */
static inline int64_t* aitisa_tensor_dims(const Tensor t) {
  return t->shape.dims;
}

/**
 * @brief Get the total number of elements of the tensor.
 */
static inline int64_t aitisa_tensor_size(const Tensor t) {
  return t->size;
}

/**
 * @brief Get the shape of the tensor.
 */
static inline Shape aitisa_tensor_shape(const Tensor t) {
  return t->shape;
}

/**
 * @brief Get the layout type of the tensor.
 */
static inline LayoutType aitisa_tensor_layout_type(const Tensor t) {
  return t->shape.layout.type;
}

/**
 * @brief Get the storage of the tensor.
 */
static inline Storage aitisa_tensor_storage(const Tensor t) {
  return t->storage;
}

/**
 * @brief Get the the data type of the tensor.
 */
static inline DataType aitisa_tensor_data_type(const Tensor t) {
  return t->storage->dtype;
}


/**
 * @brief Get the raw data pointer of the tensor.
 */
static inline void* aitisa_tensor_data(const Tensor t) {
  return t->storage->data;
}

/**
 * @brief Get the device of the tensor.
 */
static inline Device aitisa_tensor_device(const Tensor t) {
  return t->storage->device;
}

/**
 * @brief Set the item in the specified position of the tensor.
 */
static inline void aitisa_tensor_set_item(const Tensor t, int64_t idx,
                                          void *value) {
  char *data = (char *)aitisa_tensor_data(t);
  char *ptr = data + idx * aitisa_tensor_data_type(t).size;
  memcpy(ptr, value, aitisa_tensor_data_type(t).size);
  //aisisa_set_typed_value_func(aitisa_tensor_data_type(t))((void *)ptr, value);
}

/**
 * @brief Create a new tensor by allocating the memory space and setting its
 *        arributes.
 *
 * @param dtype The data type of the tensor
 * @param device The device of the tensor
 * @param dims The dimensions of the tensor
 * @param ndim The number of dimensions of the tensor
 * @param data The pointer of data of the tensor
 * @param len The length of data, also the number of bytes of data
 * @param output The created tensor pointer
 * @return Status The status indicates whether the function launched
 *                successfully.
 */
AITISA_API_PUBLIC Status aitisa_create(DataType dtype, Device device, 
                                       int64_t *dims, int64_t ndim, 
                                       void *data, unsigned int len, 
                                       Tensor *output);

/**
 * @brief Destroy the tensor by deallocating all related members.
 *
 * @param input The tensor to be destoried.
 * @return Status The status indicates whether the function launched
 *                successfully.
 */
AITISA_API_PUBLIC Status aitisa_destroy(Tensor *input);

/**
 * @brief Resolve a tensor into DataType, Device, dims, ndim, data and len.
 *
 * @param input The tensor to be resolved
 * @param dtype The data type of the tensor
 * @param device The device of the tensor
 * @param dims The dimensions of the tensor
 * @param ndim The number of dimensions of the tensor
 * @param data The pointer of data of the tensor
 * @param len The length of data, also the number of bytes of data
 * @return Status The status indicates whether the function launched
 *                successfully.
 */
AITISA_API_PUBLIC Status aitisa_resolve(Tensor input, DataType *dtype, 
                                        Device *device, int64_t **dims,
                                        int64_t *ndim, void **data, 
                                        unsigned int *len);

#endif
