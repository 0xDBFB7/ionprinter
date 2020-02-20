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

__host__ void CUDA_simple_copy_test(){
    const int N = 10;

    float * x = new float[N];
    for(int i = 0; i < N; i++){ x[i]= i;};
    float * d_x;

    gpu_error_check(cudaMalloc(&d_x, N*sizeof(float)));
                    //always dest, src
    gpu_error_check(cudaMemcpy(d_x, x, N*sizeof(float), cudaMemcpyHostToDevice));
    gpu_error_check(cudaDeviceSynchronize());

    gpu_error_check(cudaMemcpy(x, d_x, N*sizeof(float), cudaMemcpyDeviceToHost));
    gpu_error_check( cudaDeviceSynchronize() );

    pretty_print_array(x, 0, N);

    gpu_error_check(cudaFree(d_x));

    delete [] x;
}

struct test_struct{
    int test_int[10];
    float * storage;
};

__global__ void test_fill_simple(test_struct * d_a) {
    for(int i = 0; i < 10; i++){
        d_a->storage[i] = d_a->test_int[5];
    }
    d_a->test_int[5] = 15;
}



__host__ void CUDA_simple_struct_copy_test(){



    // gpu_error_check(cudaMalloc((void**)&(device->storage), N*sizeof(float)));
    // cudaMemcpy(&(device->storage), &(d_arr), sizeof(int*), cudaMemcpyHostToDevice); //copy the pointer value itself
    //
    const int N = 10;

    test_struct * host_input = new test_struct;
    host_input->test_int[5] = 10;
    host_input->storage = new float[N];
    for(int i = 0; i < N; i++){ host_input->storage[i] = i;};


    //copy the struct, plus values on the stack
    test_struct * device;
    gpu_error_check(cudaMalloc((void**)&device, sizeof(test_struct)));
    gpu_error_check(cudaMemcpy(device, host_input, sizeof(test_struct), cudaMemcpyHostToDevice));

    float * device_storage;
    gpu_error_check(cudaMalloc(&device_storage, N*sizeof(float)));
    //copy the data
    gpu_error_check(cudaMemcpy(device_storage, host_input->storage, N*sizeof(float), cudaMemcpyHostToDevice));
    //bind - copy the pointer itself
    gpu_error_check(cudaMemcpy(&(device->storage), &device_storage, sizeof(device->storage), cudaMemcpyHostToDevice));


    test_fill_simple<<<1, 1>>>(device);
    gpu_error_check( cudaPeekAtLastError() );
    gpu_error_check( cudaDeviceSynchronize() );

    delete [] host_input->storage;

    //then copy the struct itself
    gpu_error_check(cudaMemcpy(host_input, device, sizeof(test_struct), cudaMemcpyDeviceToHost));
    //but of course now the pointers are overwritten with device pointers, so we need to fix that:
    float * host_output_storage = new float[N];
    host_input->storage = host_output_storage;

    //copy the data back
    float * device_output_storage;
    //copy the pointer to the data
    gpu_error_check(cudaMemcpy(&device_output_storage,&(device->storage), sizeof(device->storage), cudaMemcpyDeviceToHost));
    //then the data itself
    gpu_error_check(cudaMemcpy(host_input->storage, device_output_storage, N*sizeof(float), cudaMemcpyDeviceToHost));



    pretty_print_array(host_input->storage, 0, N);
    pretty_print_array(host_input->test_int, 0, N);

    // //
    // gpu_error_check(cudaMemcpy(host_input, device, sizeof(test_struct), cudaMemcpyDeviceToHost));
    // gpu_error_check(cudaMemcpy(host_input->storage, device->storage, N*sizeof(float), cudaMemcpyDeviceToHost));
    //
    //
    //
    // gpu_error_check(cudaFree(device->storage));
    // gpu_error_check(cudaFree(device));
    //
    delete [] host_input->storage;
    delete host_input;
}


__host__ void CUDA_struct_copy_test(){
//     //cuda Unified Memory with ManagedMemcpy would be much simpler,
//     //but I think I prefer to see precisely when a PCIe transfer will occur.
//
//     int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 5, 5};
//     //input, which we don't want to mess with
//     physics_mesh * host_input = new physics_mesh(mesh_sizes, 1);
//     //so we create a surrogate to copy values to, without destructors
//     physics_mesh * host_b = new physics_mesh(1);
//     physics_mesh * device_a;
//
//     host_input->potential[10] = 100;
//     //copy all the physics arrays
//     POTENTIAL_TYPE * device_potential;
//     gpu_error_check(cudaMalloc(&device_potential, MESH_BUFFER_SIZE*sizeof(POTENTIAL_TYPE)));
//                         //order is (device, host
//     gpu_error_check(cudaMemcpy(device_potential, host_input->potential, MESH_BUFFER_SIZE*sizeof(POTENTIAL_TYPE), cudaMemcpyHostToDevice));
//
//     //this step has to be done before the struct is copied to the device
//     host_b->potential = device_potential;
//
//     //copy the struct itself (and accessory array values on the stack)
//     gpu_error_check(cudaMalloc(&device_a, sizeof(physics_mesh)));
//     gpu_error_check(cudaMemcpy(device_a, host_b, sizeof(physics_mesh), cudaMemcpyHostToDevice));
//
//     test_fill<<<1, 1>>>(*device_a);
//     gpu_error_check( cudaPeekAtLastError() );
//     gpu_error_check( cudaDeviceSynchronize() );
//
//
//     // //and back again
//     // physics_mesh * host_output = new physics_mesh(1); // no destructors!
//     // gpu_error_check(cudaMemcpy(host_output, device_a, sizeof(physics_mesh), cudaMemcpyDeviceToHost));
//     POTENTIAL_TYPE * output_potential = new POTENTIAL_TYPE[MESH_BUFFER_SIZE];
//
//     gpu_error_check(cudaMemcpy(output_potential, device_a->potential, MESH_BUFFER_SIZE*sizeof(POTENTIAL_TYPE), cudaMemcpyDeviceToHost));
//
//
//     cudaFree(device_potential);
//     cudaFree(device_a->potential);
//     cudaFree(device_a);
//     delete host_b;
//     delete host_input;
//
//     // delete [] host_output->potential;
//     // delete host_output;
//     // delete [] device_a->potential;
//     delete [] output_potential;
//
}
//
