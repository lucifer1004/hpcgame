Standard APIs for AI operations

# Environment Requirement

| Dependency | Version required    |
| ---------- | ------------------- |
| gcc        | `5.0` or higher     |
| CMake      | `3.11` or higher    |

# Setup
You can setup *Standard APIs for AI operations* by following instructions:
1. Use **git clone** instruction to download source code

      ```bash
      git clone https://github.com/PKUHPC/aitisa_api_sample.git
      ```

2. Make a new directory **build** under the project directory, then use **cmake** instruction

      ```bash
      mkdir build  
      cd build  
      cmake ..
      ```

3. Use **make** instruction to compile the code

      ```bash
      make
      ```
      
4. Run testing file. Take running convolution operator testing file as an example

      ```bash
      cd bin
      ./conv2d_simple_test 
      ```

-------

# Operator performance optimization
Here are three commonly used operators in the field of deep learning, and what you need to do is optimize them to improve their performance.

## matmul
Calculate the result of multiplying two 2D matrices. 

You can modify the macro MM_KERNEL_SIMPLE in /src/math/matmul_simple.c to optimize the operator.

If you want to test the operator yourself, you can run this

```bash
cd build
cmake ..
make
cd bin
./matmul_simple_test ../../test/test_data/matmul/2_case_32_1024_32_float/input1.dat ../../test/test_data/matmul/2_case_32_1024_32_float/input2.dat ../../test/test_data/matmul/2_case_32_1024_32_float/output.dat
```


## conv2d
Calculate two-dimensional convolution based on 4D input tensor(N, C_in, H, W) and 4D filter(C_in, C_out, K, K).

You can modify the macro CONV2D_KERNEL_SIMPLE in /src/nn/conv2d_simple.c to optimize the operator.

If you want to test the operator yourself, you can run this

```bash
cd build
cmake ..
make
cd bin
./conv2d_simple_test ../../test/test_data/conv2d/1_case_2_2_16_16_float/input.dat  ../../test/test_data/conv2d/1_case_2_2_16_16_float/filter.dat ../../test/test_data/conv2d/1_case_2_2_16_16_float/output.dat
```

## resize2d_bilinear
Resize 2D images with bilinear interpolation.

You can modify the macro resize2d_bilinear_kernel in /src/nn/resize2d_bilinear.c to optimize the operator.

If you want to test the operator yourself, you can run this

```bash
cd build
cmake ..
make
cd bin
./resize2d_bilinear_test ../../test/test_data/resize2d_bilinear/1_case_1024_1024_512_512_float/input.dat ../../test/test_data/resize2d_bilinear/1_case_1024_1024_512_512_float/output.dat  ../../test/test_data/resize2d_bilinear/1_case_1024_1024_512_512_float/shape.dat  
```

## References

- [talhasaruhan/cpp-matmul](https://github.com/talhasaruhan/cpp-matmul)
