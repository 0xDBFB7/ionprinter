#include "nyion.hpp"
#include "visualize.hpp"
#include "host_data_structure.hpp"
#include "device_data_structure.hpp"

#include "host_data_structure_test.hpp"


#include "gtest/gtest.h"


TEST(CUDA, CUDA_display_info)
{
  display_GPU_info();
}
//
// TEST(CUDA, CUDA_struct_copy_test){
//     int mesh_sizes[MAX_DEPTH] = {3, 3, 3};
//     physics_mesh a(mesh_sizes);
//     physics_mesh * d_a;
//
//
//     gpu_error_check(cudaMalloc(&d_a, sizeof(physics_mesh)));
//     cudaMemcpy(d_a, a, sizeof(physics_mesh), cudaMemcpyHostToDevice);
//
//     add<<<1, 1>>>(d_a);
//
//     cudaFree(d_a);
// }
