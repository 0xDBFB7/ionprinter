//make -j16 && /usr/local/cuda-10.2/bin/nvprof ./test/nyion_test
//make -j16 && /usr/local/cuda-10.2/bin/cuda-memcheck --leak-check full ./test/nyion_test

//sudo fuser -v /dev/nvidia* tells you what programs are using the GPU
//https://stackoverflow.com/questions/9309195/copying-a-struct-containing-pointers-to-cuda-device
//the input to a kernel can actually be a host structure
//as long as all the pointers therein are to device arrays.
//good to know! see also
// --- Destructor (correct version)
//  __host__ __device__ ~Lock(void) {
//#if !defined(__CUDACC__)
//      gpuErrchk(cudaFree(d_state));
//#else
//
//#endif
//  }
//note:
//Max block dimensions: [ 1024, 1024, 64 ]
//Max grid dimensions:  [ 2147483647, 65535, 65535 ]
// the grid is fine, but we might have more than 1024 blocks!


// cuda Unified Memory with ManagedMemcpy would be much simpler,
// but I think I prefer to see precisely when a PCIe transfer will occur.

//unified memory allows multiple GPUs, which might be useful down the road.

/* @landaverde
"We find that for the vast majority of applications,
UMA generates significant overhead and results in notable performance loss.

Furthermore, the UMA model only marginally simplifies the programming model for most applications.
"
*/

//cool.
