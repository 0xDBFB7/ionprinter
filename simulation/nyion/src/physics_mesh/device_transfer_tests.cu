#include "physics_mesh.hpp"

#include "device_transfer_tests.hpp"

#include "gtest/gtest.h"

#include "nyion.hpp"


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

TEST(CUDA, physics_mesh_device_copy_test2){

    int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 5, 5};
    physics_mesh origin_host(mesh_sizes, 1);
    physics_mesh * host_struct = &origin_host;
//
    for(int i = 0; i < origin_host.buffer_end_pointer; i++){ origin_host.potential[i] = 10+i;};

    physics_mesh * device_struct;

    physics_mesh::device_constructor(&device_struct);

    physics_mesh::copy_to_device(&device_struct, &host_struct);
    //we do this twice to check if our pointers were preserved correctly - seperate into other test

    //run kernel
    physics_test_fill_simple<<<1, 1>>>(*device_struct);
    gpu_error_check( cudaPeekAtLastError() );
    gpu_error_check( cudaDeviceSynchronize() );

    physics_mesh::copy_to_host(&device_struct, &host_struct);
    physics_mesh::copy_to_host(&device_struct, &host_struct);


    ASSERT_NEAR(origin_host.potential[0],110,1e-3);

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


TEST(CUDA, CUDA_device_copy_ghost_values){
    int mesh_sizes[MESH_BUFFER_DEPTH] = {4,5};
    physics_mesh origin_host(mesh_sizes,2);

    origin_host.refine_cell(0, 21); //refine two non-ghost blocks
    origin_host.refine_cell(0, 22); //adjacent in +x
    origin_host.refine_cell(0, 37); //adjacent in +z
    origin_host.set_level_ghost_linkages(1);

    origin_host.potential[220] = 3.14;
    origin_host.potential[280] = 3.14*2;
    origin_host.potential[345] = 3.14*3;

    physics_mesh * host_struct = &origin_host;
    physics_mesh * device_struct;
    physics_mesh::device_constructor(&device_struct);
    physics_mesh::copy_to_device(&device_struct, &host_struct);

    physics_mesh::device_copy_ghost_values(host_struct, device_struct, &((*device_struct).potential), 1);

    physics_mesh::copy_to_host(&device_struct, &host_struct);

    cudaDeviceSynchronize();

    ASSERT_NEAR(origin_host.potential[98],3.14,1e-2); //points to 220
    ASSERT_NEAR(origin_host.potential[158],3.14*2,1e-2); //points to 280
    ASSERT_NEAR(origin_host.potential[170],3.14*3,1e-2); //points to 345
    physics_mesh::device_destructor(&device_struct);
}

TEST(CUDA, CUDA_device_jacobi_kernel_1){
    int mesh_sizes[MESH_BUFFER_DEPTH] = {4,5};
    physics_mesh origin_host(mesh_sizes,2);

    origin_host.potential[21] = 1;

    physics_mesh * host_struct = &origin_host;
    physics_mesh * device_struct;
    physics_mesh::device_constructor(&device_struct);
    physics_mesh::copy_to_device(&device_struct, &host_struct);

    physics_mesh::device_jacobi_relax(host_struct, device_struct, &((*device_struct).potential), 0);

    physics_mesh::copy_to_host(&device_struct, &host_struct);

    ASSERT_NEAR(origin_host.potential[22],(1.0/6.0),1e-3); //points to 220
}


TEST(CUDA, CUDA_size_blocks_1){
    int mesh_sizes[MESH_BUFFER_DEPTH] = {4};
    physics_mesh origin_host(mesh_sizes,1);
    physics_mesh * host_struct = &origin_host;

    dim3 threads;
    dim3 blocks;
    set_GPU_dimensions(host_struct,blocks,threads,0);

    ASSERT_EQ(blocks.PHYSICAL_BLOCKS,1);
    ASSERT_EQ(blocks.SUB_BLOCKS,1);

    ASSERT_EQ(threads.x,2); //-2 ghosts
    ASSERT_EQ(threads.y,2);
    ASSERT_EQ(threads.z,2);
}


TEST(CUDA, CUDA_size_blocks_2){
    int mesh_sizes[MESH_BUFFER_DEPTH] = {34};
    physics_mesh origin_host(mesh_sizes,1);
    physics_mesh * host_struct = &origin_host;

    dim3 threads;
    dim3 blocks;
    set_GPU_dimensions(host_struct,blocks,threads,0);

    ASSERT_EQ(blocks.PHYSICAL_BLOCKS,1);
    ASSERT_EQ(blocks.SUB_BLOCKS,32);

    ASSERT_EQ(threads.x,32);
    ASSERT_EQ(threads.y,32);
    ASSERT_EQ(threads.z,1);
}


//
// TEST(CUDA, CUDA_device_jacobi_kernel_benchmark){
//     int mesh_sizes[MESH_BUFFER_DEPTH] = {50,5};
//     physics_mesh origin_host(mesh_sizes,2);
//
//     origin_host.potential[21] = 1;
//
//     physics_mesh * host_struct = &origin_host;
//     physics_mesh * device_struct;
//     physics_mesh::device_constructor(&device_struct);
//     physics_mesh::copy_to_device(&device_struct, &host_struct);
//
//     auto start = std::chrono::high_resolution_clock::now();
//
//     for(int i = 0; i < 10; i++){
//         physics_mesh::device_jacobi_relax(host_struct, device_struct, &((*device_struct).potential), 0);
//     }
//
//     auto end = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::microseconds>( end-start ).count()/100.0;
//     std::cout << duration << " us, " << "\n";
//
//     physics_mesh::copy_to_host(&device_struct, &host_struct);
//
//     ASSERT_NEAR(origin_host.potential[22],1/6,1e-4); //points to 220
// }


void link_cuda(); //forces CMAKE to link cuda test code.
                    // there's probably a much more elegant way to do this.
