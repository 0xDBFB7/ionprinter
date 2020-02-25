#include "device_transfer_tests.hpp"

#include "nyion.hpp"

#include "physics_mesh.hpp"

__global__ void physics_test_fill_simple(physics_mesh &mesh) {
    for(int i = 0; i < mesh.buffer_end_pointer; i++){
        mesh.potential[i] += i+100;
        mesh.boundary_conditions[i] += i+100;

    }
}

TEST(CUDA, physics_mesh_device_copy_test){

    int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 5, 5};
    physics_mesh origin_host(mesh_sizes, 1);
    physics_mesh * host_struct = &origin_host;
//
    for(int i = 0; i < origin_host.buffer_end_pointer; i++){ origin_host.potential[i] = 10+i;};
    for(int i = 0; i < origin_host.buffer_end_pointer; i++){ origin_host.boundary_conditions[i] = 10+i;};

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

    // pretty_print_array(origin_host.potential, 0, 20);
    // origin_host.pretty_print();

    ASSERT_NEAR(origin_host.potential[0],110,1e-3);
    ASSERT_NEAR(origin_host.boundary_conditions[0],110,1e-3);
    ASSERT_NEAR(origin_host.potential[origin_host.buffer_end_pointer+1],0,1e-3);
    //testing the zero case, to catch garbage on initialization
    ASSERT_NEAR(origin_host.mesh_sizes[0],3,1e-3);

    physics_mesh::device_destructor(&device_struct);
}




__global__ void refine_on_device(physics_mesh &mesh) {
    mesh.refine_cell(0,0);
}

TEST(CUDA, CUDA_refine_on_device){
    int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 5, 5};
    physics_mesh origin_host(mesh_sizes, 1);
    physics_mesh * host_struct = &origin_host;

    physics_mesh * device_struct;
    physics_mesh::device_constructor(&device_struct);

    physics_mesh::copy_to_device(&device_struct, &host_struct);

    //run kernel
    refine_on_device<<<1, 1>>>(*device_struct);
    gpu_error_check( cudaPeekAtLastError() );
    gpu_error_check( cudaDeviceSynchronize() );

    physics_mesh::copy_to_host(&device_struct, &host_struct);

    cudaDeviceSynchronize();

    ASSERT_EQ(origin_host.refined_indices[0],cube(3));

    physics_mesh::device_destructor(&device_struct);
}


void link_cuda(); //forces CMAKE to link cuda test code.
                    // there's probably a much more elegant way to do this.
