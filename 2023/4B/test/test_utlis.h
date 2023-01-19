#include <vector>
#include "test/test_data/read_and_write_data.h"
extern "C" {
#include "src/basic/factories.h"
}

template <class datatype>
void assign(Tensor t, std::vector<datatype> input_data) {
  int64_t size = aitisa_tensor_size(t);
  auto* tensor_data = (float*)aitisa_tensor_data(t);
  for (int i = 0; i < size; ++i) {
    tensor_data[i] = input_data[i];
  }
}

const DataType aitisa_dtypes[10] = {kInt8,   kUint8, kInt16,  kUint16, kInt32,
                                    kUint32, kInt64, kUint64, kFloat,  kDouble};
inline DataType aitisa_int_to_dtype(int n) {
  return aitisa_dtypes[n];
}