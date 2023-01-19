#ifndef STORAGE_H
#define STORAGE_H

#include <stdint.h>
#include <stdlib.h>
#include "src/core/device.h"
#include "src/core/status.h"
#include "src/core/types.h"

/**
 * @brief The implementation of the storage hold by the tensor
 */
struct _StorageImpl {
  DataType dtype; /**< The data type of the elements in the storage */
  Device device;  /**< The device where the elements are stored */
  int64_t size;   /**< The number of the elements hold by the storage */
  void *data;     /**< The raw data pointer pointing to the memory space */
};

/* The storage handle for users without exposing the implementation details. */
typedef struct _StorageImpl *Storage;

/**
 * @brief Create a new storage using the specific parameters
 *
 * @param dtype The data type of storage
 * @param device The device to create storage on
 * @param size The total number of elements that will put in this storage
 * @param data The pointer of data of the storage
 * @param storage A new storage to be created
 *
 * @code
 * Storage storage;
 * DataType dtype = kInt32;   // equivalent with: dtype = {TYPE_INT32, sizeof(int)};
 * Device device = {DEVICE_CPU, 0};
 * aitisa_create_storage(dtype, device, 12, storage);
 * @endcode
 *
 * @return
 * @retval STATUS_SUCCESS Successfully create a new storage
 * @retval STATUS_ALLOC_FAILED Failed when the storage already exists
 */
Status aitisa_create_storage(DataType dtype, Device device, int64_t size,
                             void *data, Storage *storage);

/**
 * @brief Destroy an exist storage
 *
 * @param storage the storage to be destroy
 *
 * @return
 * @retval STATUS_SUCCESS Successfully destroy a storage
 */
Status aitisa_destroy_storage(Storage *storage);

#endif
