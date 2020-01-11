/*

Here's the situation.

Having a tree relationship between levels and blocks makes some kind of sense.
Everything multigrid is naturally tree-based,
having a structure to traverse is great for cell-world and world-cell lookups, etc.
It's also easier to code the construction of meshes.

However, traversing the tree takes jumps and is generally poorly optimized.
Once the trees are established on the heap, however, a simple list of indices
can be constructed to traverse linearly.


//block_list
//block_sizes
//connections

//
//Ryzen 7: 0.25 TF/40 gbps.
https://en.wikichip.org/wiki/amd/ryzen_7/1700
//
//GTX 1060 is 3 TFlops/160 gbps,
//An RTX 2070 Super hits 8.2 TF/448.0 gbps, and 21 to 51 TFlops half precision.
//V100 hits 14TF/900 +100TF half.
//https://en.wikipedia.org/wiki/List_of_Nvidia_graphics_processing_units
//
//
*/

#include "unrolled_operations.hpp"

//Special thanks to http://ianfinlayson.net/class/cpsc425/notes/cuda-random!


#define N 10

#define MAX 100


// __device__ float curand_uniform (curandState_t *state)
// __device__ float curand_normal (curandState_t *state)
//                  curand(&state)
__global__
void add()
{
  // int i = blockIdx.x*blockDim.x + threadIdx.x;
  // x[0] = 0;
  // y[0] = 100;
}


void test_cuda(float * x)
{

  // const int N = 10;
  // float *d_x, *d_y;
  // //
  // gpu_error_check( cudaMalloc(&d_x, N*sizeof(float)));
  // gpu_error_check( cudaMalloc(&d_y, N*sizeof(float)) );
  // //
  // // cudaMemcpy(d_x, x, N*sizeof(float), cudaMemcpyHostToDevice);
  // // cudaMemcpy(d_y, y, N*sizeof(float), cudaMemcpyHostToDevice);

  // Perform SAXPY on 1M elements
  // gpu_error_check( add<<<1, 1>>>(); );


  // DisplayHeader();
  // cudaMemcpy(x, d_x, N*sizeof(float), cudaMemcpyDeviceToHost);
  // cudaMemcpy(y, d_y, N*sizeof(float), cudaMemcpyDeviceToHost);
  //
  // cudaFree(d_x);
  // cudaFree(d_y);
}
