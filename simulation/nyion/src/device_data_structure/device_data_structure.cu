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


//
// void test_cuda_struct(){
//     physics_mesh a;
//     physics_mesh d_a;
//     int mesh_sizes[MESH_BUFFER_DEPTH] = {3, 3, 3};
//
//     a = physics_mesh(mesh_sizes);
//
//     gpu_error_check( cudaMalloc(&d_a, sizeof(physics_mesh)));
//     cudaMemcpy(d_a, a, sizeof(physics_mesh), cudaMemcpyHostToDevice);
//
//     add<<<1, 1>>>(d_a);
//
//     cudaFree(d_a);
//     free(a);
// }





void test_cuda(float * x)
{

  // const int N = 10;
  // float *d_x, *d_y;
  // //
  // gpu_error_check( cudaMalloc(&d_x, N*sizeof(float)));
  // gpu_error_check( cudaMalloc(&d_y, N*sizeof(float)) );
  // //
  // // cudaMemcpy(d_x, x, N*sizeof(float), cudaMemcpyHostToDevice);
  // // cudaMemcpy(d_y, y, N*sizeof(float), cudaMemcpyHostToDevice
  // Perform SAXPY on 1M elements
  // gpu_error_check( add<<<1, 1>>>(); );

  // gpu_error_check( add<<<1, 1>>>(); );

  // DisplayHeader();
  // cudaMemcpy(x, d_x, N*sizeof(float), cudaMemcpyDeviceToHost);
  // cudaMemcpy(y, d_y, N*sizeof(float), cudaMemcpyDeviceToHost);
  //
  // cudaFree(d_x);
  // cudaFree(d_y);
}