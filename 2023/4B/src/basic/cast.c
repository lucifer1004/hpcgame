#include "src/basic/cast.h"
#include "src/core/dispatch.h"

#define cast_kernel(out_typename, in_typename, in_data, out_data, size) \
  in_typename *typed_in_data = in_data;                                 \
  out_typename *typed_out_data = out_data;                              \
  for (int64_t i = 0; i < size; i++) {                                  \
    typed_out_data[i] = (out_typename)(typed_in_data[i]);               \
  }

static Status cast_template(const Tensor input, DataType out_dtype,
                            Tensor *output) {
  DataType in_dtype = aitisa_tensor_data_type(input);
  int64_t size = aitisa_tensor_size(input);
  void *in_data = aitisa_tensor_data(input);
  void *out_data = aitisa_tensor_data(*output);
  switch (in_dtype.code) {
    case TYPE_INT8: {
      AITISA_DISPATCH_ALL_TYPES_RETURN(out_dtype, cast_kernel, int8_t, in_data,
                                                 out_data, size);
      break;
    }
    case TYPE_UINT8: {
      AITISA_DISPATCH_ALL_TYPES_RETURN(out_dtype, cast_kernel, uint8_t, in_data,
                                                 out_data, size);
      break;
    }
    case TYPE_INT16: {
      AITISA_DISPATCH_ALL_TYPES_RETURN(out_dtype, cast_kernel, int16_t, in_data,
                                                 out_data, size);
      break;
    }
    case TYPE_UINT16: {
      AITISA_DISPATCH_ALL_TYPES_RETURN(out_dtype, cast_kernel, uint16_t, in_data,
                                                 out_data, size);
      break;
    }
    case TYPE_INT32: {
      AITISA_DISPATCH_ALL_TYPES_RETURN(out_dtype, cast_kernel, int32_t, in_data,
                                                 out_data, size);
      break;
    }
    case TYPE_UINT32: {
      AITISA_DISPATCH_ALL_TYPES_RETURN(out_dtype, cast_kernel, uint32_t, in_data,
                                                 out_data, size);
      break;
    }
    case TYPE_INT64: {
      AITISA_DISPATCH_ALL_TYPES_RETURN(out_dtype, cast_kernel, int64_t, in_data,
                                                 out_data, size);
      break;
    }
    case TYPE_UINT64: {
      AITISA_DISPATCH_ALL_TYPES_RETURN(out_dtype, cast_kernel, uint64_t, in_data,
                                                 out_data, size);
      break;
    }
    case TYPE_FLOAT: {
      AITISA_DISPATCH_ALL_TYPES_RETURN(out_dtype, cast_kernel, float, in_data,
                                                 out_data, size);
      break;
    }
    case TYPE_DOUBLE: {
      AITISA_DISPATCH_ALL_TYPES_RETURN(out_dtype, cast_kernel, double, in_data,
                                                 out_data, size);
      break;
    }
    default:
      return STATUS_NOT_SUPPORTED;
  }
  return STATUS_SUCCESS;
}

Status aitisa_cast(const Tensor input, DataType dtype, Tensor *output) {
  // If the DataType of input and the DataType to cast to are identical,
  // then do nothing.
  DataType in_dtype = aitisa_tensor_data_type(input);
  if (in_dtype.code == dtype.code) {
    return STATUS_SUCCESS;
  }
  // Create output
  Tensor new_tensor;
  Device device = aitisa_tensor_device(input);
  // LayoutType layout_type = aitisa_tensor_layout_type(input);
  int64_t *out_dims = aitisa_tensor_dims(input);
  int64_t out_ndim = aitisa_tensor_ndim(input);
  CHECK_STATUS(aitisa_create(dtype, device, out_dims, out_ndim,
                             NULL, 0, &new_tensor));
  *output = new_tensor;
  // Implement cast
  Status status;
  status = cast_template(input, dtype, output);
  return status;
}
