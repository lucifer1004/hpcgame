#ifndef STATUS_H 
#define STATUS_H

typedef enum {
  STATUS_SUCCESS,
  STATUS_TYPE_MISMATCH,
  STATUS_DIMENSIONS_MISMATCH,
  STATUS_INVALID_ARGUMENT,
  STATUS_NOT_SUPPORTED,
  STATUS_ALLOC_FAILED,
  STATUS_OUT_OF_RANGE,
  STATUS_INTERNAL_ERROR,
  STATUS_MATH_ERROR
} Status;

#define CHECK_STATUS(status)           \
  do {                                 \
    Status s = status;                 \
    if (s != STATUS_SUCCESS) return s; \
  } while (0)

#endif