#include "src/math/matmul_simple.h"
#include "src/basic/broadcast.h"
#include "src/basic/factories.h"
#include "src/core/allocator.h"
#include "src/core/dispatch.h"
#include "src/core/utils.h"

// kernel of matrix-matrix multiply
#ifndef BLOCK_SIZE
#define BLOCK_SIZE 16
#endif

#define MM_KERNEL_SIMPLE(typename, A, B, C, M, K, N)                     \
  typename* BT = (typename*)malloc(K * N * sizeof(typename));            \
  for (int i = 0; i < N; i++) {                                          \
    for (int j = 0; j < K; j++) {                                        \
      BT[i * K + j] = ((typename*)B)[j * N + i];                         \
    }                                                                    \
  }                                                                      \
                                                                         \
  int i = 0;                                                             \
  for (; i < M - BLOCK_SIZE; i += BLOCK_SIZE) {                          \
    int j = 0;                                                           \
    for (; j < N - BLOCK_SIZE; j += BLOCK_SIZE) {                        \
      for (int row = 0; row < BLOCK_SIZE; row++) {                       \
        for (int col = 0; col < BLOCK_SIZE; col++) {                     \
          int r = i + row;                                               \
          int c = j + col;                                               \
          int aoff = r * K;                                              \
          int boff = c * K;                                              \
          typename tmp = 0;                                              \
          for (int q = 0; q < K; ++q) {                                  \
            tmp += ((typename*)A)[aoff + q] * ((typename*)BT)[boff + q]; \
          }                                                              \
          ((typename*)C)[r * N + c] = tmp;                               \
        }                                                                \
      }                                                                  \
    }                                                                    \
    for (int row = 0; row < BLOCK_SIZE; row++) {                         \
      for (int c = j; c < N; c++) {                                      \
        int r = i + row;                                                 \
        int aoff = r * K;                                                \
        int boff = c * K;                                                \
        typename tmp = 0;                                                \
        for (int q = 0; q < K; ++q) {                                    \
          tmp += ((typename*)A)[aoff + q] * ((typename*)BT)[boff + q];   \
        }                                                                \
        ((typename*)C)[r * N + c] = tmp;                                 \
      }                                                                  \
    }                                                                    \
  }                                                                      \
  for (; i < M; i++) {                                                   \
    for (int j = 0; j < N; j++) {                                        \
      int aoff = i * K;                                                  \
      int boff = j * K;                                                  \
      typename tmp = 0;                                                  \
      for (int q = 0; q < K; ++q) {                                      \
        tmp += ((typename*)A)[aoff + q] * ((typename*)BT)[boff + q];     \
      }                                                                  \
      ((typename*)C)[i * N + j] = tmp;                                   \
    }                                                                    \
  }                                                                      \
  free(BT);

// choose mm kernel according to dtype.code
Status mm_simple_template(DataType dtype, void* A, void* B, void* C, int64_t M,
                          int64_t K, int64_t N) {
  AITISA_DISPATCH_ALL_TYPES_RETURN(dtype, MM_KERNEL_SIMPLE, A, B, C, M, K, N);
  return STATUS_SUCCESS;
}

// Definition of aitisa_matmul.
Status aitisa_matmul_simple(const Tensor tensor1, const Tensor tensor2,
                            Tensor* output) {
  if (aitisa_tensor_device(tensor1).type != DEVICE_CPU ||
      aitisa_tensor_device(tensor2).type != DEVICE_CPU) {
    return STATUS_NOT_SUPPORTED;
  }
  int64_t ndim_tensor1 = aitisa_tensor_ndim(tensor1);
  int64_t ndim_tensor2 = aitisa_tensor_ndim(tensor2);
  Status status = STATUS_SUCCESS;
  if (ndim_tensor1 == 2 && ndim_tensor2 == 2) {
    // matrix-matrix
    int64_t dim0_tensor1 = aitisa_tensor_dim(tensor1, 0);
    int64_t dim1_tensor1 = aitisa_tensor_dim(tensor1, 1);
    int64_t dim0_tensor2 = aitisa_tensor_dim(tensor2, 0);
    int64_t dim1_tensor2 = aitisa_tensor_dim(tensor2, 1);
    if (dim1_tensor1 != dim0_tensor2) {
      return STATUS_DIMENSIONS_MISMATCH;
    }
    // create output
    int64_t ndim_out = 2;
    int64_t dims_out[2] = {dim0_tensor1, dim1_tensor2};
    CHECK_STATUS(aitisa_full(aitisa_tensor_data_type(tensor1),
                             aitisa_tensor_device(tensor1), dims_out, ndim_out,
                             0.0, output));
    // call kernel
    CHECK_STATUS(mm_simple_template(
        aitisa_tensor_data_type(tensor1), aitisa_tensor_data(tensor1),
        aitisa_tensor_data(tensor2), aitisa_tensor_data(*output), dim0_tensor1,
        dim1_tensor1, dim1_tensor2));
  } else {
    status = STATUS_INVALID_ARGUMENT;
  }
  return status;
}
