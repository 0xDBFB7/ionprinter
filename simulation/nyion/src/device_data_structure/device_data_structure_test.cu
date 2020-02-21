#include "device_data_structure.hpp"
#include "host_data_structure.hpp"


#include "gtest/gtest.h"

//make -j16 && /usr/local/cuda-10.2/bin/nvprof ./test/nyion_test
//make -j16 && /usr/local/cuda-10.2/bin/cuda-memcheck --leak-check full ./test/nyion_test

//https://stackoverflow.com/questions/9309195/copying-a-struct-containing-pointers-to-cuda-device
//the input to a kernel can actually be a host structure
//as long as all the pointers therein are to device arrays.
//good to know! see also
// --- Destructor (correct version)
//  __host__ __device__ ~Lock(void) {
//#if !defined(__CUDACC__)
//      gpuErrchk(cudaFree(d_state));
//#else
//
//#endif
//  }
//note:
//Max block dimensions: [ 1024, 1024, 64 ]
//Max grid dimensions:  [ 2147483647, 65535, 65535 ]
// the grid is fine, but we might have more than 1024 blocks!


// cuda Unified Memory with ManagedMemcpy would be much simpler,
// but I think I prefer to see precisely when a PCIe transfer will occur.

//unified memory allows multiple GPUs, which might be useful down the road.


__global__ void physics_test_fill_simple(physics_mesh * d_a) {
    for(int i = 0; i < 10; i++){
        d_a->potential[i] = 100;
    }
}

__host__ void CUDA_physics_mesh_copy_test(){
    int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 5, 5};
    physics_mesh origin_host(mesh_sizes, 1);
    physics_mesh * host_struct = &origin_host;

    for(int i = 0; i < 10; i++){ origin_host.potential[i] = i;};

    physics_mesh * device_struct;

    physics_mesh::device_constructor(&device_struct); //s
    //double pointer required to preserve malloc edit

    physics_mesh::copy_to_device_struct(&device_struct, &host_struct);
    physics_mesh::copy_to_device_struct(&device_struct, &host_struct); //check if our pointers were preserved


    //run kernel
    physics_test_fill_simple<<<1, 1>>>(device_struct);
    gpu_error_check( cudaPeekAtLastError() );
    gpu_error_check( cudaDeviceSynchronize() );


    physics_mesh::copy_to_host_struct(&device_struct, &host_struct);
    physics_mesh::copy_to_host_struct(&device_struct, &host_struct);

    pretty_print_array(origin_host.potential, 0, 20);

    origin_host.pretty_print();

    ASSERT_NEAR(origin_host.potential[0],100,1e-3);
    ASSERT_NEAR(origin_host.mesh_sizes[0],3,1e-3);
    //remember to test the zero case, to catch garbage on initialization

    physics_mesh::device_destructor(&device_struct);

}
