#include "resize2d_bilinear.h"
#include "math.h"
#include "src/core/dispatch.h"
#define resize2d_bilinear_kernel(typename)                                 \
  typename* in_data = aitisa_tensor_data(input);                           \
  typename* out_data = aitisa_tensor_data(*output);                        \
  double dh = (double)h / (double)target_h;                                \
  double dw = (double)w / (double)target_w;                                \
  short* rh = (short*)malloc(sizeof(short) * target_h);                     \
  short* rw = (short*)malloc(sizeof(short) * target_w);                     \
  double* xh = (double*)malloc(sizeof(double) * target_h);                  \
  double* yw = (double*)malloc(sizeof(double) * target_w);                  \
  double raw_h = 0.0, raw_w = 0.0;                                         \
  for (int64_t i = 0; i < target_h; i++) {                                 \
    rh[i] = (short)raw_h;                                                  \
    xh[i] = raw_h - rh[i];                                                    \
    raw_h += dh;                                                           \
  }                                                                        \
                                                                           \
  for (int64_t i = 0; i < target_w; i++) {                                 \
    rw[i] = (short)raw_w;                                                  \
    yw[i] = raw_w - rw[i];                                                    \
    raw_w += dw;                                                           \
  }                                                                        \
                                                                           \
  short u, v;                                                              \
  double x, y;                                                             \
  typename f00, f01, f10, f11;                                             \
  for (int64_t i = 0; i < target_h; i++) {                                 \
    if (rh[i] + 1 >= h) {                                                  \
      for (int64_t j = 0; j < target_w; j++) {                             \
        out_data[i * target_w + j] = in_data[(h - 1) * w + rw[j]];         \
      }                                                                    \
      continue;                                                            \
    }                                                                      \
                                                                           \
    u = rh[i];                                                             \
    x = xh[i];                                                             \
    for (int64_t j = 0; j < target_w; j++) {                               \
      if (rw[j] + 1 >= w) {                                                \
        out_data[i * target_w + j] = in_data[rh[i] * w + w - 1];           \
        continue;                                                          \
      }                                                                    \
      v = rw[j];                                                           \
      y = yw[j];                                                           \
      f00 = in_data[u * w + v];                                            \
      f01 = in_data[u * w + v + 1];                                        \
      f10 = in_data[(u + 1) * w + v];                                      \
      f11 = in_data[(u + 1) * w + v + 1];                                  \
      out_data[i * target_w + j] = f00 * (1 - x) * (1 - y) +               \
                                   f01 * (1 - x) * y + f10 * x * (1 - y) + \
                                   f11 * x * y;                            \
    }                                                                      \
  }

Status aitisa_resize2d_bilinear(const Tensor input, int target_h, int target_w,
                                Tensor* output) {
  int64_t* dims = aitisa_tensor_dims(input);
  int64_t ndim = aitisa_tensor_ndim(input);
  Status status = STATUS_SUCCESS;
  DataType dtype = aitisa_tensor_data_type(input);
  switch (dtype.code) {
    case TYPE_FLOAT:
      break;
    case TYPE_DOUBLE:
      break;
    default:
      return STATUS_NOT_SUPPORTED;
  }
  if (ndim == 2)  // [H, W]
  {
    int h = dims[0];
    int w = dims[1];

    int64_t output_dims[2] = {target_h, target_w};

    Tensor new_tensor;
    DataType dtype = aitisa_tensor_data_type(input);
    Device device = aitisa_tensor_device(input);

    CHECK_STATUS(
        aitisa_create(dtype, device, output_dims, ndim, NULL, 0, &new_tensor));
    *output = new_tensor;
    int64_t size = aitisa_tensor_size(input);
    AITISA_DISPATCH_ALL_TYPES_RETURN(dtype, resize2d_bilinear_kernel);
  } else {
    status = STATUS_TYPE_MISMATCH;
  }
  return status;
}
