//#include "unrolled_operations.hpp"
// #include "nyion.hpp"

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

// #include "unrolled_operations.hpp"

#include <iostream>

using namespace std;

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
}

void DisplayHeader()
{
    const int kb = 1024;
    const int mb = kb * kb;
    wcout << "NBody.GPU" << endl << "=========" << endl << endl;

    wcout << "CUDA version:   v" << CUDART_VERSION << endl;

    int devCount;
    cudaGetDeviceCount(&devCount);
    wcout << "CUDA Devices: " << endl << endl;

    for(int i = 0; i < devCount; ++i)
    {
        cudaDeviceProp props;
        cudaGetDeviceProperties(&props, i);
        wcout << i << ": " << props.name << ": " << props.major << "." << props.minor << endl;
        wcout << "  Global memory:   " << props.totalGlobalMem / mb << "mb" << endl;
        wcout << "  Shared memory:   " << props.sharedMemPerBlock / kb << "kb" << endl;
        wcout << "  Constant memory: " << props.totalConstMem / kb << "kb" << endl;
        wcout << "  Block registers: " << props.regsPerBlock << endl << endl;

        wcout << "  Warp size:         " << props.warpSize << endl;
        wcout << "  Threads per block: " << props.maxThreadsPerBlock << endl;
        wcout << "  Max block dimensions: [ " << props.maxThreadsDim[0] << ", " << props.maxThreadsDim[1]  << ", " << props.maxThreadsDim[2] << " ]" << endl;
        wcout << "  Max grid dimensions:  [ " << props.maxGridSize[0] << ", " << props.maxGridSize[1]  << ", " << props.maxGridSize[2] << " ]" << endl;
        wcout << endl;
    }
}

void test_cuda()
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
  // printf("aaaaaaaaaaaaaaaaaaaa\n");

  DisplayHeader();
  // cudaMemcpy(x, d_x, N*sizeof(float), cudaMemcpyDeviceToHost);
  // cudaMemcpy(y, d_y, N*sizeof(float), cudaMemcpyDeviceToHost);
  //
  // cudaFree(d_x);
  // cudaFree(d_y);
}
