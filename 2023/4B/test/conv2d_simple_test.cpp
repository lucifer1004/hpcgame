#include "test_utlis.h"
extern "C" {
#include "src/nn/conv2d_simple.h"
}

int main(int argc, char** argv) {
  if (argc == 4) {
    Tensor input, filter, output;
    Device device = {DEVICE_CPU, 0};
    int64_t input_ndim, filter_ndim, result_ndim, input_dtype, filter_dtype,
        result_dtype, input_num, filter_num, result_num;
    std::vector<int64_t> input_dims_vector, filter_dims_vector,
        result_dims_vector;
    std::vector<float> input_data_vector, filter_data_vector,
        result_data_vector;
    read_data(argv[1], &input_ndim, &input_dims_vector, &input_dtype,
              &input_num, &input_data_vector);
    read_data(argv[2], &filter_ndim, &filter_dims_vector, &filter_dtype,
              &filter_num, &filter_data_vector);

    int64_t* input_dims;
    input_dims = (int64_t*)malloc(sizeof(int64_t) * input_ndim);
    memset(input_dims, 0, sizeof(int64_t) * input_ndim);
    for (int i = 0; i < input_ndim; i++) {
      input_dims[i] = input_dims_vector[i];
    }

    int64_t* filter_dims;
    filter_dims = (int64_t*)malloc(sizeof(int64_t) * filter_ndim);
    memset(filter_dims, 0, sizeof(int64_t) * filter_ndim);
    for (int i = 0; i < filter_ndim; i++) {
      filter_dims[i] = filter_dims_vector[i];
    }

    aitisa_create(aitisa_int_to_dtype(input_dtype), device, input_dims,
                  input_ndim, nullptr, 0, &input);
    aitisa_create(aitisa_int_to_dtype(filter_dtype), device, filter_dims,
                  filter_ndim, nullptr, 0, &filter);
    assign(input, input_data_vector);
    assign(filter, filter_data_vector);
    aitisa_conv2d_simple(input, filter, &output);
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
    free(input_dims);
    free(filter_dims);

    aitisa_destroy(&input);
    aitisa_destroy(&filter);
    aitisa_destroy(&output);
  } else if (argc > 4) {
    std::cout << "Too many arguments supplied." << std::endl;
    exit(1);
  } else {
    std::cout
        << "Three argument expected. Usage: ./conv2d_simple_test <input_path> "
           "<filter_path> <result_path> "
        << std::endl;
    exit(1);
  }
}
