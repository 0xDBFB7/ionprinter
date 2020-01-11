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

#include "unrolled_operations.hpp"

//Special thanks to http://ianfinlayson.net/class/cpsc425/notes/cuda-random!


#define N 10

#define MAX 100

/* this GPU kernel function is used to initialize the random states */
__global__ void init(unsigned int seed, curandState_t* states) {

  /* we have to initialize the state */
  curand_init(seed, /* the seed can be the same for each core, here we pass the time in from the CPU */
              blockIdx.x, /* the sequence number should be different for each core (unless you want all
                             cores to get the same sequence of numbers for some reason - use thread id! */
              0, /* the offset is how much extra we advance in the sequence for each call, can be 0 */
              &states[blockIdx.x]);
}

/* this GPU kernel takes an array of states, and an array of ints, and puts a random int into each */
__global__ void randoms(curandState_t* states, unsigned int* numbers) {
  /* curand works like rand - except that it takes a state as a parameter */
  numbers[blockIdx.x] = curand_uniform(&states[blockIdx.x]) * MAX;
}

int random_numbers() {
  /* CUDA's random number library uses curandState_t to keep track of the seed value
     we will store a random state for every thread  */
  curandState_t* states;

  /* allocate space on the GPU for the random states */
  cudaMalloc((void**) &states, N * sizeof(curandState_t));

  /* invoke the GPU to initialize all of the random states */
  init<<<N, 1>>>(time(0), states);

  /* allocate an array of unsigned ints on the CPU and GPU */
  unsigned int cpu_nums[N];
  unsigned int* gpu_nums;
  cudaMalloc((void**) &gpu_nums, N * sizeof(unsigned int));

  /* invoke the kernel to get some random numbers */
  randoms<<<N, 1>>>(states, gpu_nums);

  /* copy the random numbers back */
  cudaMemcpy(cpu_nums, gpu_nums, N * sizeof(unsigned int), cudaMemcpyDeviceToHost);

  /* print them out */
  for (int i = 0; i < N; i++) {
    printf("%u\n", cpu_nums[i]);
  }

  /* free the memory we allocated for the states and numbers */
  cudaFree(states);
  cudaFree(gpu_nums);

  return 0;
}
//
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

  random_numbers();

  // DisplayHeader();
  // cudaMemcpy(x, d_x, N*sizeof(float), cudaMemcpyDeviceToHost);
  // cudaMemcpy(y, d_y, N*sizeof(float), cudaMemcpyDeviceToHost);
  //
  // cudaFree(d_x);
  // cudaFree(d_y);
}
