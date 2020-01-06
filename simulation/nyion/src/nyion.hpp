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

#include <mgl2/mgl.h>

#endif
