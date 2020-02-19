#include "device_data_structure.hpp"
#include "host_data_structure.hpp"




void CUDA_struct_copy_test(){

    int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 5, 5};
    physics_mesh a(mesh_sizes, 1);

    physics_mesh * d_a;

    gpu_error_check(cudaMalloc(&d_a, sizeof(physics_mesh)));
    cudaMemcpy(d_a, &a, sizeof(physics_mesh), cudaMemcpyHostToDevice);

    add_<<<1, 1>>>();
    //<<<block # in level, cube(mesh_sizes[level])>>>
    //both can be dim3

    cudaFree(d_a);
}
