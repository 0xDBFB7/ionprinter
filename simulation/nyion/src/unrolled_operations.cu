//#include "unrolled_operations.hpp"
#include "nyion.hpp"

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

*/

#define gpu_error_check(ans) { gpuAssert((ans), __FILE__, __LINE__); } //thanks to talonmies!
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true)
{
   if (code != cudaSuccess)
   {
      fprintf(stderr,"GPU error: %s %s %d\n", cudaGetErrorString(code), file, line);
      if (abort) exit(code);
   }
}


__global__
void add()
{
  // int i = blockIdx.x*blockDim.x + threadIdx.x;
  // x[0] = 0;
  // y[0] = 100;
  printf("test\n");
}


void test_cuda(float * x, float * y)
{

  // int N = 10;
  // float *d_x, *d_y;
  // //
  // gpu_error_check( cudaMalloc(&d_x, N*sizeof(float)));
  // cudaMalloc(&d_y, N*sizeof(float));
  // //
  // // cudaMemcpy(d_x, x, N*sizeof(float), cudaMemcpyHostToDevice);
  // // cudaMemcpy(d_y, y, N*sizeof(float), cudaMemcpyHostToDevice);

  // Perform SAXPY on 1M elements
  add<<<1, 1>>>();


  // cudaMemcpy(x, d_x, N*sizeof(float), cudaMemcpyDeviceToHost);
  // cudaMemcpy(y, d_y, N*sizeof(float), cudaMemcpyDeviceToHost);
  //
  // cudaFree(d_x);
  // cudaFree(d_y);
}
