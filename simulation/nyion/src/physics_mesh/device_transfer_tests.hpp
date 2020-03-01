#ifndef UNROLLED_H
#define UNROLLED_H

#include "nyion.hpp"

#include "CUDA_helpers.hpp"

void CUDA_refine_on_device();
void CUDA_physics_mesh_device_copy_test2();
void CUDA_physics_mesh_device_copy_test();

TEST(CUDA, CUDA_refine_on_device){
    CUDA_refine_on_device();
}

TEST(CUDA, physics_mesh_device_copy_test2){
    CUDA_physics_mesh_device_copy_test2();
}

TEST(CUDA, physics_mesh_device_copy_test){
    CUDA_physics_mesh_device_copy_test();
}

void link_cuda();

#endif
