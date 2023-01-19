# Add unit test files
list(APPEND aitisa_test_srcs
  matmul_simple_test.cpp
  conv2d_simple_test.cpp
  resize2d_bilinear_test.cpp
)

# Build a exe for each test files
foreach(test_src ${aitisa_test_srcs})
  get_filename_component(test_name ${test_src} NAME_WE)
  add_executable(${test_name} ${test_src})
  target_link_libraries(${test_name} "aitisa_api::aitisa_api")
endforeach()