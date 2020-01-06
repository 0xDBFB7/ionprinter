#include "unrolled_operations.hpp"

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

__global__
void cuda_hello(){
    printf("Hello World from GPU!\n");
}

int test_cuda(){
  cuda_hello<<<1,1>>>();
  cudaDeviceSynchronize();

  return 0;
}
