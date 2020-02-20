/*

Having a tree relationship between with levels and blocks makes some kind of sense.
Everything multigrid is naturally tree-based,
having a structure to traverse is great for cell-world and world-cell lookups,
and to generate the linkages between ghosts.
It's also easier to code the construction of meshes.

However, traversing the tree takes jumps and is generally poorly optimized,
especially on a GPU, where a jump stalls the warp.
Once the trees are established on the heap, however, a simple list of indices
can be constructed to traverse linearly.

It might be possible to do away with the tree entirely, but
trying to visualize 'neighbors' with arbitrarily placed blocks
does my head in.

It's important to note that this entire implementation completely disregards
the cache altogether. The indice system means the next block could be halfway to Manitoba
out of cache; there's no cache concurrency, congruency, or congealency; the
&mesh argument means we're probably bringing 20x as much data along as we need for each function;
etc.

If one were truly smart, one could probably eek the same performance out of a
homogenous computing system. I am not.

//block_list
//block_sizes
//connections

//
//Ryzen 7: 0.25 TF/40 gbps. https://en.wikichip.org/wiki/amd/ryzen_7/1700
//GTX 1060: 3 TFlops/160 GBps.
//RTX 2070 Super: 8.2 TF/448.0 GBps - 21 to 51 TFlops half precision.
//V100 hits 14TF/900 GBps + 100TF half.
//https://en.wikipedia.org/wiki/List_of_Nvidia_graphics_processing_units
//
//
*/

#include "device_data_structure.hpp"
#include "host_data_structure.hpp"

//Special thanks to http://ianfinlayson.net/class/cpsc425/notes/cuda-random!


#define N 10

#define MAX 100

// __device__ float curand_uniform (curandState_t *state)
// __device__ float curand_normal (curandState_t *state)
//                  curand(&state)
__global__ void add_() {

  // int i = blockIdx.x*blockDim.x + threadIdx.x;
  // x[0] = 0;
  // y[0] = 100;
}

__host__ void construct_device_physics_mesh(physics_mesh * device_physics_mesh){
    gpu_error_check(cudaMalloc((void**)&device_physics_mesh, sizeof(physics_mesh)));


}

// 
// __host__ void copy_physics_mesh_to_gpu(physics_mesh * host_physics_mesh, physics_mesh * device_physics_mesh){
//     //remember to zero allocated arrays!
//     //override new with zero constructor?
//
//     //copy struct and data on stack
//     gpu_error_check(cudaMemcpy(device_physics_mesh, host_physics_mesh, sizeof(physics_mesh), cudaMemcpyHostToDevice));
//
//
//     float * device_storage;
//     gpu_error_check(cudaMalloc(&device_storage, N*sizeof(float)));
//     // //copy the data
//     gpu_error_check(cudaMemcpy(device_storage, host_input->storage, N*sizeof(float), cudaMemcpyHostToDevice));
//     // //bind - copy the pointer itself
//     gpu_error_check(cudaMemcpy(&(device->storage), &device_storage, sizeof(device->storage), cudaMemcpyHostToDevice));
// }
//
//


void test_cuda(float * x){
}
