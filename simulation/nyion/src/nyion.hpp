#ifndef NYION_H
#define NYION_H

// _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);

int idx(int x, int y, int z, int xlen);

#include <iostream>
#include <string>

#include <chrono>
#include <math.h>
#include <dbg.h>

#include <omp.h>

#include "debug_helpers.hpp"


#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#include <cuda.h>

#include <curand.h>
#include <curand_kernel.h>

#include "CUDA_helpers.hpp"

#define gpu_error_check(ans) { gpuAssert((ans), __FILE__, __LINE__); } //thanks to talonmies!


#include <mgl2/mgl.h>

#endif
