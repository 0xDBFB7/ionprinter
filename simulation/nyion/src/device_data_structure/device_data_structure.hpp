#ifndef UNROLLED_H
#define UNROLLED_H

#include "nyion.hpp"

void test_cuda(float * x);
void test_cuda_struct();

__global__ void add_();

void copy_physics_mesh_to_gpu();

#endif
