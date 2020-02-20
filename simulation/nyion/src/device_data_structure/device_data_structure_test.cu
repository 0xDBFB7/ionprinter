#include "device_data_structure.hpp"
#include "host_data_structure.hpp"

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

__global__ void test_fill(physics_mesh &d_a) {
    for(int i = 0; i < 10; i++){
        d_a.potential[i] = i;
    }
}



__host__ void CUDA_struct_copy_test(){
    //cuda Unified Memory with ManagedMemcpy would be much simpler,
    //but I think I prefer to see precisely when a PCIe transfer will occur.


    int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 5, 5};
    //input, which we don't want to mess with
    physics_mesh * host_input = new physics_mesh(mesh_sizes, 1);
    //so we create a surrogate to copy values to, without destructors
    physics_mesh * host_b = new physics_mesh(1);
    physics_mesh * device_a;

    host_input->potential[10] = 100;
    //copy all the physics arrays
    POTENTIAL_TYPE * device_potential;
    gpu_error_check(cudaMalloc(&device_potential, MESH_BUFFER_SIZE*sizeof(POTENTIAL_TYPE)));
                        //order is (device, host
    gpu_error_check(cudaMemcpy(device_potential, host_input->potential, MESH_BUFFER_SIZE*sizeof(POTENTIAL_TYPE), cudaMemcpyHostToDevice));

    //this step has to be done before the struct is copied to the device
    host_b->potential = device_potential;

    //copy the struct itself (and accessory array values on the stack)
    gpu_error_check(cudaMalloc(&device_a, sizeof(physics_mesh)));
    gpu_error_check(cudaMemcpy(device_a, host_b, sizeof(physics_mesh), cudaMemcpyHostToDevice));

    test_fill<<<1, 1>>>(*device_a);
    gpu_error_check( cudaPeekAtLastError() );
    gpu_error_check( cudaDeviceSynchronize() );


    // //and back again
    // physics_mesh * host_output = new physics_mesh(1); // no destructors!
    // gpu_error_check(cudaMemcpy(host_output, device_a, sizeof(physics_mesh), cudaMemcpyDeviceToHost));
    POTENTIAL_TYPE * output_potential;
    // host_output->potential =
    gpu_error_check(cudaMemcpy(output_potential, device_a->potential, MESH_BUFFER_SIZE*sizeof(POTENTIAL_TYPE), cudaMemcpyDeviceToHost));


    cudaFree(device_potential);
    cudaFree(device_a->potential);
    cudaFree(device_a);
    delete host_b;
    delete host_input;

    // delete [] host_output->potential;
    // delete host_output;
    // delete [] device_a->potential;
    delete output_potential;

}
