#include "src/core/types.h"

#include <stdint.h>

/**
 * Define DataType constants for all types.
 * looks like:
      const DataType kInt8 = {TYPE_INT8, sizeof(int8_t)};
 */
#define AITISA_DEFINE_CONSTANTS_DATA_TYPE(Dtype, DTYPE, dtype) \
  const DataType k##Dtype = {TYPE_##DTYPE, sizeof(dtype)};

AITISA_DEFINE_CONSTANTS_DATA_TYPE(Int8, INT8, int8_t);
AITISA_DEFINE_CONSTANTS_DATA_TYPE(Uint8, UINT8, uint8_t);
AITISA_DEFINE_CONSTANTS_DATA_TYPE(Int16, INT16, int16_t);
AITISA_DEFINE_CONSTANTS_DATA_TYPE(Uint16, UINT16, uint16_t);
AITISA_DEFINE_CONSTANTS_DATA_TYPE(Int32, INT32, int32_t);
AITISA_DEFINE_CONSTANTS_DATA_TYPE(Uint32, UINT32, uint32_t);
AITISA_DEFINE_CONSTANTS_DATA_TYPE(Int64, INT64, int64_t);
AITISA_DEFINE_CONSTANTS_DATA_TYPE(Uint64, UINT64, uint64_t);
AITISA_DEFINE_CONSTANTS_DATA_TYPE(Float, FLOAT, float);
AITISA_DEFINE_CONSTANTS_DATA_TYPE(Double, DOUBLE, double);
#undef AITISA_DEFINE_CONSTANTS_DATA_TYPE

/**
 * Define get_type_value functions for all types.
 * looks like:
      void get_int8_value(void *addr, void *value) {
        *(int8_t *)value = *(int8_t *)addr;
      }
 */
#define AITISA_DEFINE_FUNC_GET_TYPE_VALUE(type, type_t) \
  void get_##type##_value(void *addr, void *value) {    \
    *(type_t *)value = *(type_t *)addr;                 \
  }

AITISA_DEFINE_FUNC_GET_TYPE_VALUE(int8, int8_t);
AITISA_DEFINE_FUNC_GET_TYPE_VALUE(uint8, uint8_t);
AITISA_DEFINE_FUNC_GET_TYPE_VALUE(int16, int16_t);
AITISA_DEFINE_FUNC_GET_TYPE_VALUE(uint16, uint16_t);
AITISA_DEFINE_FUNC_GET_TYPE_VALUE(int32, int32_t);
AITISA_DEFINE_FUNC_GET_TYPE_VALUE(uint32, uint32_t);
AITISA_DEFINE_FUNC_GET_TYPE_VALUE(int64, int64_t);
AITISA_DEFINE_FUNC_GET_TYPE_VALUE(uint64, uint64_t);
AITISA_DEFINE_FUNC_GET_TYPE_VALUE(float, float);
AITISA_DEFINE_FUNC_GET_TYPE_VALUE(double, double);
#undef AITISA_DEFINE_FUNC_GET_TYPE_VALUE

/**
 * Define set_type_value functions for all types.
 * looks like:
      void set_int8_value(void *addr, void *value) {
        *(int8_t *)addr = *(int8_t *)value;
      }
 */
#define AITISA_DEFIEN_FUNC_SET_TYPE_VALUE(type, type_t) \
  void set_##type##_value(void *addr, void *value) {    \
    *(type_t *)addr = *(type_t *)value;                 \
  }

AITISA_DEFIEN_FUNC_SET_TYPE_VALUE(int8, int8_t);
AITISA_DEFIEN_FUNC_SET_TYPE_VALUE(uint8, uint8_t);
AITISA_DEFIEN_FUNC_SET_TYPE_VALUE(int16, int16_t);
AITISA_DEFIEN_FUNC_SET_TYPE_VALUE(uint16, uint16_t);
AITISA_DEFIEN_FUNC_SET_TYPE_VALUE(int32, int32_t);
AITISA_DEFIEN_FUNC_SET_TYPE_VALUE(uint32, uint32_t);
AITISA_DEFIEN_FUNC_SET_TYPE_VALUE(int64, int64_t);
AITISA_DEFIEN_FUNC_SET_TYPE_VALUE(uint64, uint64_t);
AITISA_DEFIEN_FUNC_SET_TYPE_VALUE(float, float);
AITISA_DEFIEN_FUNC_SET_TYPE_VALUE(double, double);
#undef AITISA_DEFIEN_FUNC_SET_TYPE_VALUE

/**
 * Define castto_type_values functions for all types.
 * looks like:
      void castto_int8_value(void *addr, double *value) {
        *(int8_t *)addr = *value;
      }
 */
#define AITISA_DEFINE_FUNC_CAST_TO_TYPE_VALUE(type, type_t) \
  void castto_##type##_value(void *addr, double *value) {   \
    *(type_t *)addr = *value;                               \
  }

AITISA_DEFINE_FUNC_CAST_TO_TYPE_VALUE(int8, int8_t);
AITISA_DEFINE_FUNC_CAST_TO_TYPE_VALUE(uint8, uint8_t);
AITISA_DEFINE_FUNC_CAST_TO_TYPE_VALUE(int16, int16_t);
AITISA_DEFINE_FUNC_CAST_TO_TYPE_VALUE(uint16, uint16_t);
AITISA_DEFINE_FUNC_CAST_TO_TYPE_VALUE(int32, int32_t);
AITISA_DEFINE_FUNC_CAST_TO_TYPE_VALUE(uint32, uint32_t);
AITISA_DEFINE_FUNC_CAST_TO_TYPE_VALUE(int64, int64_t);
AITISA_DEFINE_FUNC_CAST_TO_TYPE_VALUE(uint64, uint64_t);
AITISA_DEFINE_FUNC_CAST_TO_TYPE_VALUE(float, float);
AITISA_DEFINE_FUNC_CAST_TO_TYPE_VALUE(double, double);
#undef AITISA_DEFINE_FUNC_CAST_TO_TYPE_VALUE

/**
 * Define get_type_array_value functions for all types.
 * looks like:
      void get_int8_array_value(void *addr, int64_t i, void *value) {
        *(int8_t *)value = ((int8_t *)addr)[i];
      }
 */
#define AITISA_DEFINE_FUNC_GET_TYPE_ARRAY_VALUE(type, type_t)         \
  void get_##type##_array_value(void *addr, int64_t i, void *value) { \
    *(type_t *)value = ((type_t *)addr)[i];                           \
  }

AITISA_DEFINE_FUNC_GET_TYPE_ARRAY_VALUE(int8, int8_t);
AITISA_DEFINE_FUNC_GET_TYPE_ARRAY_VALUE(uint8, uint8_t);
AITISA_DEFINE_FUNC_GET_TYPE_ARRAY_VALUE(int16, int16_t);
AITISA_DEFINE_FUNC_GET_TYPE_ARRAY_VALUE(uint16, uint16_t);
AITISA_DEFINE_FUNC_GET_TYPE_ARRAY_VALUE(int32, int32_t);
AITISA_DEFINE_FUNC_GET_TYPE_ARRAY_VALUE(uint32, uint32_t);
AITISA_DEFINE_FUNC_GET_TYPE_ARRAY_VALUE(int64, int64_t);
AITISA_DEFINE_FUNC_GET_TYPE_ARRAY_VALUE(uint64, uint64_t);
AITISA_DEFINE_FUNC_GET_TYPE_ARRAY_VALUE(float, float);
AITISA_DEFINE_FUNC_GET_TYPE_ARRAY_VALUE(double, double);
#undef AITISA_DEFINE_FUNC_GET_TYPE_ARRAY_VALUE

/**
 * Define set_type_array_value functions for all types.
 * looks like:
      void set_int8_array_value(void *addr, int64_t i, void *value) {
        ((int8_t *)addr)[i] = *(int8_t *)value;
      }
 */
#define AITISA_DEFINE_FUNC_SET_TYPE_ARRAY_VALUE(type, type_t)         \
  void set_##type##_array_value(void *addr, int64_t i, void *value) { \
    ((type_t *)addr)[i] = *(type_t *)value;                           \
  }

AITISA_DEFINE_FUNC_SET_TYPE_ARRAY_VALUE(int8, int8_t);
AITISA_DEFINE_FUNC_SET_TYPE_ARRAY_VALUE(uint8, uint8_t);
AITISA_DEFINE_FUNC_SET_TYPE_ARRAY_VALUE(int16, int16_t);
AITISA_DEFINE_FUNC_SET_TYPE_ARRAY_VALUE(uint16, uint16_t);
AITISA_DEFINE_FUNC_SET_TYPE_ARRAY_VALUE(int32, int32_t);
AITISA_DEFINE_FUNC_SET_TYPE_ARRAY_VALUE(uint32, uint32_t);
AITISA_DEFINE_FUNC_SET_TYPE_ARRAY_VALUE(int64, int64_t);
AITISA_DEFINE_FUNC_SET_TYPE_ARRAY_VALUE(uint64, uint64_t);
AITISA_DEFINE_FUNC_SET_TYPE_ARRAY_VALUE(float, float);
AITISA_DEFINE_FUNC_SET_TYPE_ARRAY_VALUE(double, double);
#undef AITISA_DEFINE_FUNC_SET_TYPE_ARRAY_VALUE

GetTypedValueFunc get_typed_value_funcs[TYPE_NTYPES] = {
    get_int8_value,  get_uint8_value,  get_int16_value, get_uint16_value,
    get_int32_value, get_uint32_value, get_int64_value, get_uint64_value,
    get_float_value, get_double_value};

GetTypedArrayValueFunc get_typed_array_value_funcs[TYPE_NTYPES] = {
    get_int8_array_value,   get_uint8_array_value,  get_int16_array_value,
    get_uint16_array_value, get_int32_array_value,  get_uint32_array_value,
    get_int64_array_value,  get_uint64_array_value, get_float_array_value,
    get_double_array_value};

SetTypedValueFunc set_typed_value_funcs[TYPE_NTYPES] = {
    set_int8_value,  set_uint8_value,  set_int16_value, set_uint16_value,
    set_int32_value, set_uint32_value, set_int64_value, set_uint64_value,
    set_float_value, set_double_value};

SetTypedArrayValueFunc set_typed_array_value_funcs[TYPE_NTYPES] = {
    set_int8_array_value,   set_uint8_array_value,  set_int16_array_value,
    set_uint16_array_value, set_int32_array_value,  set_uint32_array_value,
    set_int64_array_value,  set_uint64_array_value, set_float_array_value,
    set_double_array_value};

CasttoTypedValueFunc castto_typed_value_funcs[TYPE_NTYPES] = {
    castto_int8_value,   castto_uint8_value,  castto_int16_value,
    castto_uint16_value, castto_int32_value,  castto_uint32_value,
    castto_int64_value,  castto_uint64_value, castto_float_value,
    castto_double_value};

GetTypedValueFunc aitisa_get_typed_value_func(DataType dtype) {
  return get_typed_value_funcs[dtype.code];
}

GetTypedArrayValueFunc aitisa_get_typed_array_value_func(DataType dtype) {
  return get_typed_array_value_funcs[dtype.code];
}

SetTypedValueFunc aitisa_set_typed_value_func(DataType dtype) {
  return set_typed_value_funcs[dtype.code];
}

SetTypedArrayValueFunc aitisa_set_typed_array_value_func(DataType dtype) {
  return set_typed_array_value_funcs[dtype.code];
}

CasttoTypedValueFunc aitisa_castto_typed_value_func(DataType dtype) {
  return castto_typed_value_funcs[dtype.code];
}
