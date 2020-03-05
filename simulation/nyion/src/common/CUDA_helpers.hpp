#ifndef CUDA_helpers_H
#define CUDA_helpers_H

#include <iostream>

void display_GPU_info();


#define gpu_error_check(ans) { gpuAssert((ans), __FILE__, __LINE__); } //thanks to talonmies!

inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true){
   if (code != cudaSuccess)
   {
      fprintf(stderr,"GPU error: %s %s %d\n", cudaGetErrorString(code), file, line);
      if (abort){
          cudaDeviceReset();
          exit(code);
      }
   }
}

#endif
