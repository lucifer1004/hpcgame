#include "test_utlis.h"
extern "C" {
#include "src/math/matmul_simple.h"
}
int main(int argc, char** argv) {
  if (argc == 4) {
    Tensor input1, input2, output;
    Device device = {DEVICE_CPU, 0};
    int64_t input1_ndim, input2_ndim, result_ndim, input1_dtype, input2_dtype,
        result_dtype, input1_num, input2_num, result_num;
    std::vector<int64_t> input1_dims_vector, input2_dims_vector,
        result_dims_vector;
    std::vector<float> input1_data_vector, input2_data_vector,
        result_data_vector;
    read_data(argv[1], &input1_ndim, &input1_dims_vector, &input1_dtype,
              &input1_num, &input1_data_vector);
    read_data(argv[2], &input2_ndim, &input2_dims_vector, &input2_dtype,
              &input2_num, &input2_data_vector);

    int64_t* input1_dims;
    input1_dims = (int64_t*)malloc(sizeof(int64_t) * input1_ndim);
    memset(input1_dims, 0, sizeof(int64_t) * input1_ndim);
    for (int i = 0; i < input1_ndim; i++) {
      input1_dims[i] = input1_dims_vector[i];
    }

    int64_t* input2_dims;
    input2_dims = (int64_t*)malloc(sizeof(int64_t) * input2_ndim);
    memset(input2_dims, 0, sizeof(int64_t) * input2_ndim);
    for (int i = 0; i < input2_ndim; i++) {
      input2_dims[i] = input2_dims_vector[i];
    }

    aitisa_create(aitisa_int_to_dtype(input1_dtype), device, input1_dims,
                  input1_ndim, nullptr, 0, &input1);
    aitisa_create(aitisa_int_to_dtype(input2_dtype), device, input2_dims,
                  input2_ndim, nullptr, 0, &input2);
    assign(input1, input1_data_vector);
    assign(input2, input2_data_vector);

    aitisa_matmul_simple(input1, input2, &output);
    read_data(argv[3], &result_ndim, &result_dims_vector, &result_dtype,
              &result_num, &result_data_vector);
    auto output_data = (float*)aitisa_tensor_data(output);
    int64_t output_size = aitisa_tensor_size(output);
    for (int i = 0; i < output_size; ++i) {
      //      std::cout << output_data[i] << std::endl;
      if (abs(output_data[i] - result_data_vector[i]) > 1e-3) {
        std::cout << " mismatch!" << std::endl;
        exit(1);
      }
    }
    free(input1_dims);
    free(input2_dims);

    aitisa_destroy(&input1);
    aitisa_destroy(&input2);
    aitisa_destroy(&output);
  } else if (argc > 4) {
    std::cout << "Too many arguments supplied." << std::endl;
    exit(1);
  } else {
    std::cout
        << "Three argument expected. Usage: ./matmul_simple_test <input1_path> "
           "<input2_path> <result_path> "
        << std::endl;
    exit(1);
  }
}
