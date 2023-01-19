#include "src/basic/broadcast.h"
#include <stdbool.h>
#include "src/core/allocator.h"

// Assume dims_out has been allocated memory.
Status aitisa_broadcast_array(int64_t* dims_in1, int64_t ndim_in1,
                              int64_t* dims_in2, int64_t ndim_in2,
                              int64_t* dims_out, int64_t ndim_out) {
  int64_t i = 0;
  while (i < ndim_in1 && i < ndim_in2) {
    int64_t d_in1 = dims_in1[ndim_in1 - i - 1];
    int64_t d_in2 = dims_in2[ndim_in2 - i - 1];
    bool match = d_in1 == 1 || d_in2 == 1 || d_in1 == d_in2;
    if (match) {
      dims_out[ndim_out - i - 1] = d_in1 > d_in2 ? d_in1 : d_in2;
      i++;
    } else {
      return STATUS_DIMENSIONS_MISMATCH;
    }
  }
  while (i < ndim_in1) {
    dims_out[ndim_out - i - 1] = dims_in1[ndim_in1 - i - 1];
    i++;
  }
  while (i < ndim_in2) {
    dims_out[ndim_out - i - 1] = dims_in2[ndim_in2 - i - 1];
    i++;
  }
  return STATUS_SUCCESS;
}
