#include "src/nn/conv2d_simple.h"
#include "src/basic/broadcast.h"
#include "src/basic/factories.h"
#include "src/core/allocator.h"
#include "src/core/dispatch.h"
#include "src/core/utils.h"

// kernel of conv2d_simple
#ifndef BLOCK_SIZE
#define BLOCK_SIZE 16
#endif

#define CONV2D_KERNEL_SIMPLE(typename, A, B, C, N, C_in, H, W, C_out, K)     \
  int H_out = H - K + 1;                                                     \
  int W_out = W - K + 1;                                                     \
  for (int n = 0; n < N; n++) {                                              \
    for (int c = 0; c < C_out; c++) {                                        \
      for (int i = 0; i < H_out; i++) {                                      \
        for (int j = 0; j < W_out; j++) {                                    \
          int offset_output =                                                \
              n * C_out * H_out * W_out + c * H_out * W_out + i * W_out + j; \
          typename tmp = 0.0;                                                \
          for (int kc = 0; kc < C_in; kc++) {                                \
            for (int ki = 0; ki < K; ki++) {                                 \
              int kj = 0;                                                    \
              for (; kj < K - 4; kj += 4) {                                  \
                int offset_input =                                           \
                    n * C_in * H * W + kc * H * W + (i + ki) * W + (j + kj); \
                int offset_filter =                                          \
                    c * C_in * K * K + kc * K * K + ki * K + kj;             \
                tmp += ((typename*)A)[offset_input] *                        \
                       ((typename*)B)[offset_filter];                        \
                tmp += ((typename*)A)[offset_input + 1] *                    \
                       ((typename*)B)[offset_filter + 1];                    \
                tmp += ((typename*)A)[offset_input + 2] *                    \
                       ((typename*)B)[offset_filter + 2];                    \
                tmp += ((typename*)A)[offset_input + 3] *                    \
                       ((typename*)B)[offset_filter + 3];                    \
              }                                                              \
              for (; kj < K; kj++) {                                         \
                int offset_input =                                           \
                    n * C_in * H * W + kc * H * W + (i + ki) * W + (j + kj); \
                int offset_filter =                                          \
                    c * C_in * K * K + kc * K * K + ki * K + kj;             \
                tmp += ((typename*)A)[offset_input] *                        \
                       ((typename*)B)[offset_filter];                        \
              }                                                              \
            }                                                                \
            ((typename*)C)[offset_output] = tmp;                             \
          }                                                                  \
        }                                                                    \
      }                                                                      \
    }                                                                        \
  }
// which means:
// C[n, c, i, j] += A[n, kc, i + ki, j + kj] * B[c, kc, ki, kj]

// choose conv2d kernel according to dtype.code
Status conv2d_simple_template(DataType dtype, void* A, void* B, void* C,
                              int64_t N, int64_t C_in, int64_t H, int64_t W,
                              int64_t C_out, int64_t K) {
  AITISA_DISPATCH_ALL_TYPES_RETURN(dtype, CONV2D_KERNEL_SIMPLE, A, B, C, N,
                                   C_in, H, W, C_out, K);
  return STATUS_SUCCESS;
}

// Definition of aitisa_conv2d_simple.
Status aitisa_conv2d_simple(const Tensor input, const Tensor filter,
                            Tensor* output) {
  // check device
  if (aitisa_tensor_device(input).type != DEVICE_CPU ||
      aitisa_tensor_device(filter).type != DEVICE_CPU) {
    return STATUS_NOT_SUPPORTED;
  }
  // check shape
  int64_t ndim_input = aitisa_tensor_ndim(input);
  int64_t ndim_filter = aitisa_tensor_ndim(filter);
  int64_t* dims_input = aitisa_tensor_dims(input);
  int64_t* dims_filter = aitisa_tensor_dims(filter);
  Status status = STATUS_SUCCESS;
  if (ndim_input != 4 || ndim_filter != 4 || dims_input[1] != dims_filter[1] ||
      dims_filter[2] != dims_filter[3]) {
    return STATUS_DIMENSIONS_MISMATCH;
  }

  int64_t num_batches = dims_input[0];
  int64_t in_channels = dims_input[1];
  int64_t height = dims_input[2];
  int64_t width = dims_input[3];

  int64_t out_channels = dims_filter[0];
  int64_t kernel_size = dims_filter[2];
  int64_t out_height = height - kernel_size + 1;
  int64_t out_width = width - kernel_size + 1;

  // create output
  int64_t ndim_out = 4;
  int64_t dims_out[4] = {num_batches, out_channels, out_height, out_width};
  CHECK_STATUS(aitisa_full(aitisa_tensor_data_type(input),
                           aitisa_tensor_device(input), dims_out, ndim_out, 0.0,
                           output));
  // call kernel
  CHECK_STATUS(conv2d_simple_template(
      aitisa_tensor_data_type(input), aitisa_tensor_data(input),
      aitisa_tensor_data(filter), aitisa_tensor_data(*output), num_batches,
      in_channels, height, width, out_channels, kernel_size));

  return status;
}
