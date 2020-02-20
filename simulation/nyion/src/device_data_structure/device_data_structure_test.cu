#include "device_data_structure.hpp"
#include "host_data_structure.hpp"

//https://stackoverflow.com/questions/9309195/copying-a-struct-containing-pointers-to-cuda-device
//the input to a kernel can actually be a host structure
//as long as all the pointers therein are to device arrays.
//good to know!
__global__ void test_fill(physics_mesh * d_a) {
    
}



__host__ void CUDA_struct_copy_test(){

    int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 5, 5};
    physics_mesh a(mesh_sizes, 1);

    physics_mesh * d_a;

    gpu_error_check(cudaMalloc(&d_a, sizeof(physics_mesh)));
    gpu_error_check(cudaMemcpy(d_a, &a, sizeof(physics_mesh), cudaMemcpyHostToDevice));
    gpu_error_check(cudaMalloc(d_a->potential, sizeof(POTENTIAL_BUFFER_TYPE)));
    test_fill<<<1, 1>>>(&a);
    //<<<block # in level, cube(mesh_sizes[level])>>>
    //both can be dim3
    //cuda Unified Memory with ManagedMemcpy would be simpler,
    //but I think I prefer to see precisely when a PCIe transfer will occur.

    cudaFree(d_a);
}
