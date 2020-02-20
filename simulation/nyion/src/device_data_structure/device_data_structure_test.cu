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
    uint8_t * small_storage;
    //must try different size types
};

__global__ void test_fill_simple(test_struct * d_a) {
    for(int i = 0; i < 10; i++){
        d_a->storage[i] = d_a->test_int[5];
    }
    d_a->test_int[5] = 15;
}



__host__ void CUDA_simple_struct_copy_test(){

    //Wwwwhat the fff
    //cudaMemcpy(device->storage,...)     does not work.
    //cudaMemcpy(host->storage,...)  does,
    //no matter the direction.
    //So that means the pointer device->storage must first be copied to a simple
    //float *,
    // then copied to host.
    // ??? okay
    //Wait, no, that makes perfect sense.
    //if you call device.storage from the host,
    //the host tries to find...
    //wait, but why does it work when copying to a float *?

    const int N = 10;

    test_struct * host_input = new test_struct;
    host_input->test_int[5] = 10;
    host_input->storage = new float[N];
    for(int i = 0; i < N; i++){ host_input->storage[i] = i;};

    // cudaMemset(dataGPU, 0, 1000*sizeof(int));

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

    float * host_output_storage = host_input->storage;
    //then copy the struct itself
    gpu_error_check(cudaMemcpy(host_input, device, sizeof(test_struct), cudaMemcpyDeviceToHost));
    //but of course now the host pointers are overwritten with device pointers, so we need to fix that:
    host_input->storage = host_output_storage;

    //copy the data back
    float * device_output_storage;
    //copy the pointer to the data
    gpu_error_check(cudaMemcpy(&device_output_storage,&(device->storage), sizeof(device->storage), cudaMemcpyDeviceToHost));
    //then the data itself
    gpu_error_check(cudaMemcpy(host_input->storage, device_output_storage, N*sizeof(float), cudaMemcpyDeviceToHost));


    pretty_print_array(host_input->storage, 0, N);
    pretty_print_array(host_input->test_int, 0, N);

    ASSERT_NEAR(host_input->storage[0],10,1e-3);
    ASSERT_NEAR(host_input->test_int[5],15,1e-3);
    //ASSERT_NEAR(host_input->test_int[0],0,1e-3); //zero test case!


    // gpu_error_check(cudaFree(device->storage));
    // gpu_error_check(cudaFree(device));

    delete [] host_input->storage;
    delete host_input;
}

void construct_device_struct(test_struct ** device_struct){
    //construct the struct
    gpu_error_check(cudaMalloc(device_struct, sizeof(test_struct)));

    float * device_storage;
    gpu_error_check(cudaMalloc(&device_storage, 10*sizeof(* device_storage)));
    gpu_error_check(cudaMemset(device_storage,0,10*sizeof(* device_storage))); //must memset the whole array!
    //copy pointer to array into struct
    gpu_error_check(cudaMemcpy(&((**device_struct).storage), &device_storage, sizeof((**device_struct).storage), cudaMemcpyHostToDevice));
}

void copy_to_device_struct(test_struct ** device_struct, test_struct ** host_struct){
    float * device_storage; //get the pointer from the device
    gpu_error_check(cudaMemcpy(&device_storage, &((**device_struct).storage), sizeof(((**device_struct).storage)), cudaMemcpyDeviceToHost));
    //and now copy the data.
    gpu_error_check(cudaMemcpy(device_storage, (**host_struct).storage,  10*sizeof(* device_storage), cudaMemcpyHostToDevice));

    //copy struct itself, wiping all the pointers,
    gpu_error_check(cudaMemcpy(*device_struct, *host_struct, sizeof(test_struct), cudaMemcpyHostToDevice));

    //then re-copy the pointers.
    gpu_error_check(cudaMemcpy(&((**device_struct).storage), &device_storage, sizeof((**device_struct).storage), cudaMemcpyHostToDevice));
    //There's a PCIe latency issue here, since we're going * -> host, data -> device,
    //but whatever!
}


void copy_to_host_struct(test_struct ** device_struct, test_struct ** host_struct){

    float * device_output_storage;
    //copy the pointer to the data
    gpu_error_check(cudaMemcpy(&device_output_storage, &((**device_struct).storage), sizeof(((**device_struct).storage)), cudaMemcpyDeviceToHost));
    //then the data itself
    gpu_error_check(cudaMemcpy((**host_struct).storage, device_output_storage, 10*sizeof(* device_output_storage), cudaMemcpyDeviceToHost));

    //save for after the wipe
    float * host_storage = (**host_struct).storage;

    //copy struct itself, wiping all the host pointers,
    gpu_error_check(cudaMemcpy(*host_struct, *device_struct, sizeof(test_struct), cudaMemcpyDeviceToHost));

    (**host_struct).storage = host_storage;
}

__host__ void CUDA_simple_struct_copy_test_2(){

    const int N = 10;

    test_struct origin_host_struct;
    test_struct * host_struct = &origin_host_struct;

    (*host_struct).test_int[5] = 10;
    (*host_struct).storage = new float[N];
    for(int i = 0; i < N; i++){ (*host_struct).storage[i] = i;};

    test_struct * device_struct;
    construct_device_struct(&device_struct); //s
    //double pointer required to preserve malloc edit

    copy_to_device_struct(&device_struct, &host_struct);
    copy_to_device_struct(&device_struct, &host_struct); //check if our pointers were preserved

    test_fill_simple<<<1, 1>>>(device_struct);
    gpu_error_check( cudaPeekAtLastError() );
    gpu_error_check( cudaDeviceSynchronize() );

    copy_to_host_struct(&device_struct, &host_struct);
    copy_to_host_struct(&device_struct, &host_struct);

    pretty_print_array(host_struct->storage, 0, N);
    pretty_print_array(host_struct->test_int, 0, N);

    ASSERT_NEAR(host_struct->storage[0],10,1e-3);
    ASSERT_NEAR(host_struct->test_int[5],15,1e-3);

    delete [] (*host_struct).storage;
    //copy the struct, plus values on the stack


    //
    // //copy the data
    // gpu_error_check(cudaMemcpy(device_storage, host_input->storage, N*sizeof(float), cudaMemcpyHostToDevice));
    // //bind - copy the pointer itself
    // gpu_error_check(cudaMemcpy(&(device->storage), &device_storage, sizeof(device->storage), cudaMemcpyHostToDevice));
    //
    // test_fill_simple<<<1, 1>>>(device);
    // gpu_error_check( cudaPeekAtLastError() );
    // gpu_error_check( cudaDeviceSynchronize() );
    //
    // delete [] host_input->storage;
    //
    // //then copy the struct itself
    // gpu_error_check(cudaMemcpy(host_input, device, sizeof(test_struct), cudaMemcpyDeviceToHost));
    // //but of course now the pointers are overwritten with device pointers, so we need to fix that:
    // float * host_output_storage = new float[N];
    // host_input->storage = host_output_storage;
    //
    // //copy the data back
    // float * device_output_storage;
    // //copy the pointer to the data
    // gpu_error_check(cudaMemcpy(&device_output_storage,&(device->storage), sizeof(device->storage), cudaMemcpyDeviceToHost));
    // //then the data itself
    // gpu_error_check(cudaMemcpy(host_input->storage, device_output_storage, N*sizeof(float), cudaMemcpyDeviceToHost));
    //
    //
    // pretty_print_array(host_input->storage, 0, N);
    // pretty_print_array(host_input->test_int, 0, N);
    //
    // ASSERT_NEAR(host_input->storage[0],10,1e-3);
    // ASSERT_NEAR(host_input->test_int[5],15,1e-3);
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
