//for consideration:
//https://google.github.io/styleguide/cppguide.html#Names_and_Order_of_Includes

#ifndef NYION_H
#define NYION_H

// _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);


#include <iostream>
#include <fstream>

#include <string>

#include <chrono>
#include <math.h>
#include <dbg.h>
#include <assert.h>


#include <omp.h>

#include "debug_helpers.hpp"

#include <stdint.h>

#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#include <cuda.h>

#include <curand.h>
#include <curand_kernel.h>

#include "CUDA_helpers.hpp"


#include <mgl2/mgl.h>

#endif
