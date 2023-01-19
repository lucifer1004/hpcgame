#ifndef DISPATCH_H
#define DISPATCH_H

#include "src/core/log.h"
#include "src/core/types.h"

#define AITISA_PRIVATE_CASE_TYPE(enum_type, func_name, ...) \
  case enum_type: {                                         \
    func_name(__VA_ARGS__);                                 \
    break;                                                  \
  }

// abort if type error
#define AITISA_DISPATCH_ALL_TYPES_ABORT(dtype, func_name, ...)      \
  switch ((dtype).code) {                                                     \
    AITISA_PRIVATE_CASE_TYPE(TYPE_INT8, func_name, int8_t, ##__VA_ARGS__)     \
    AITISA_PRIVATE_CASE_TYPE(TYPE_UINT8, func_name, uint8_t, ##__VA_ARGS__)   \
    AITISA_PRIVATE_CASE_TYPE(TYPE_INT16, func_name, int16_t, ##__VA_ARGS__)   \
    AITISA_PRIVATE_CASE_TYPE(TYPE_UINT16, func_name, uint16_t, ##__VA_ARGS__) \
    AITISA_PRIVATE_CASE_TYPE(TYPE_INT32, func_name, int32_t, ##__VA_ARGS__)   \
    AITISA_PRIVATE_CASE_TYPE(TYPE_UINT32, func_name, uint32_t, ##__VA_ARGS__) \
    AITISA_PRIVATE_CASE_TYPE(TYPE_INT64, func_name, int64_t, ##__VA_ARGS__)   \
    AITISA_PRIVATE_CASE_TYPE(TYPE_UINT64, func_name, uint64_t, ##__VA_ARGS__) \
    AITISA_PRIVATE_CASE_TYPE(TYPE_FLOAT, func_name, float, ##__VA_ARGS__)     \
    AITISA_PRIVATE_CASE_TYPE(TYPE_DOUBLE, func_name, double, ##__VA_ARGS__)   \
    default:                                                                  \
      AITISA_LOG(ERROR, "NOT_SUPPORTED_TYPE");                                \
  }

// return STATUS_NOT_SUPPORTED if type error
#define AITISA_DISPATCH_ALL_TYPES_RETURN(dtype, func_name, ...)               \
  switch ((dtype).code) {                                                     \
    AITISA_PRIVATE_CASE_TYPE(TYPE_INT8, func_name, int8_t, ##__VA_ARGS__)     \
    AITISA_PRIVATE_CASE_TYPE(TYPE_UINT8, func_name, uint8_t, ##__VA_ARGS__)   \
    AITISA_PRIVATE_CASE_TYPE(TYPE_INT16, func_name, int16_t, ##__VA_ARGS__)   \
    AITISA_PRIVATE_CASE_TYPE(TYPE_UINT16, func_name, uint16_t, ##__VA_ARGS__) \
    AITISA_PRIVATE_CASE_TYPE(TYPE_INT32, func_name, int32_t, ##__VA_ARGS__)   \
    AITISA_PRIVATE_CASE_TYPE(TYPE_UINT32, func_name, uint32_t, ##__VA_ARGS__) \
    AITISA_PRIVATE_CASE_TYPE(TYPE_INT64, func_name, int64_t, ##__VA_ARGS__)   \
    AITISA_PRIVATE_CASE_TYPE(TYPE_UINT64, func_name, uint64_t, ##__VA_ARGS__) \
    AITISA_PRIVATE_CASE_TYPE(TYPE_FLOAT, func_name, float, ##__VA_ARGS__)     \
    AITISA_PRIVATE_CASE_TYPE(TYPE_DOUBLE, func_name, double, ##__VA_ARGS__)   \
    default:                                                                  \
      return STATUS_NOT_SUPPORTED;                                            \
  }

// abort if type error
#define AITISA_DISPATCH_FLOATING_TYPES_ABORT(dtype, func_name, ...)         \
  switch ((dtype).code) {                                                   \
    AITISA_PRIVATE_CASE_TYPE(TYPE_FLOAT, func_name, float, ##__VA_ARGS__)   \
    AITISA_PRIVATE_CASE_TYPE(TYPE_DOUBLE, func_name, double, ##__VA_ARGS__) \
    default:                                                                \
      AITISA_LOG(ERROR, "NOT_SUPPORTED_TYPE");                              \
  }

// return STATUS_NOT_SUPPORTED if type error
#define AITISA_DISPATCH_FLOATING_TYPES_RETURN(dtype, func_name, ...)        \
  switch ((dtype).code) {                                                   \
    AITISA_PRIVATE_CASE_TYPE(TYPE_FLOAT, func_name, float, ##__VA_ARGS__)   \
    AITISA_PRIVATE_CASE_TYPE(TYPE_DOUBLE, func_name, double, ##__VA_ARGS__) \
    default:                                                                \
      return STATUS_NOT_SUPPORTED;                                          \
  }

#endif