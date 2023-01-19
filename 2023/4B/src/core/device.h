#ifndef DEVICE_H
#define DEVICE_H

#include "src/core/macros.h"

/**
 * @brief Enumeration type for all possible device types
 *
 * @details Use to select the right device type for tensor
 */
typedef enum {
  DEVICE_CPU = 0, /**< The tensor puts on CPU */
  DEVICE_CUDA = 1 /**< The tensor puts on GPU */
} DeviceType;

/**
 * @brief Attributes of Device struct
 *
 * @details Device structure contains all attributes which need to be known in a specific tensor
 */
typedef struct {
  DeviceType type; /**< The specific device type */
  int id; /**< The device-id among this specific device type */
} Device;

#endif
