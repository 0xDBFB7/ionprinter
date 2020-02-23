#include "nyion.hpp"

#include "physics_mesh.hpp"
// #include "traverse_state.hpp"
// #include "struct_transfer_helpers.hpp"



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


__global__ void physics_test_fill_simple(physics_mesh &mesh) {
    for(int i = 0; i < mesh.buffer_end_pointer; i++){
        mesh.potential[i] = i+100;
    }
}

void CUDA_physics_mesh_copy(){

    int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 5, 5};
    physics_mesh origin_host(mesh_sizes, 1);
    physics_mesh * host_struct = &origin_host;
//
    for(int i = 0; i < 10; i++){ origin_host.potential[i] = i;};

    physics_mesh * device_struct;

    physics_mesh::device_constructor(&device_struct);

    physics_mesh::copy_to_device(&device_struct, &host_struct);
    physics_mesh::copy_to_device(&device_struct, &host_struct);
    //we do this twice to check if our pointers were preserved correctly - seperate into other test

    //run kernel
    physics_test_fill_simple<<<1, 1>>>(*device_struct);
    gpu_error_check( cudaPeekAtLastError() );
    gpu_error_check( cudaDeviceSynchronize() );

    physics_mesh::copy_to_host(&device_struct, &host_struct);
    physics_mesh::copy_to_host(&device_struct, &host_struct);

    cudaDeviceSynchronize();

    pretty_print_array(origin_host.potential, 0, 20);

    origin_host.pretty_print();

    ASSERT_NEAR(origin_host.potential[0],100,1e-3);
    ASSERT_NEAR(origin_host.potential[origin_host.buffer_end_pointer+1],0,1e-3);
    //testing the zero case, to catch garbage on initialization
    ASSERT_NEAR(origin_host.mesh_sizes[0],3,1e-3);

    physics_mesh::device_destructor(&device_struct);

}
