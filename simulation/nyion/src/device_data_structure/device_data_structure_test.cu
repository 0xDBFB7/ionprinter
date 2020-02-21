#include "device_data_structure.hpp"
#include "host_data_structure.hpp"


#include "gtest/gtest.h"

//make -j16 && /usr/local/cuda-10.2/bin/nvprof ./test/nyion_test
//make -j16 && /usr/local/cuda-10.2/bin/cuda-memcheck ./test/nyion_test

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



__host__ void CUDA_physics_mesh_copy_test(){
//     //cuda Unified Memory with ManagedMemcpy would be much simpler,
//     //but I think I prefer to see precisely when a PCIe transfer will occur.
//
//     int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 5, 5};
//     //input, which we don't want to mess with
//     physics_mesh * host_input = new physics_mesh(mesh_sizes, 1);
//     //so we create a surrogate to copy values to, without destructors
//     physics_mesh * host_b = new physics_mesh(1);
//     physics_mesh * device_a;

}
