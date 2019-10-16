#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <chrono>
#include <vector>
#include <exception>
#include <complex>
#include <numeric>

#include <stdexcept>
#include <fstream>

#include <math.h>
#include <iostream>
#include <fstream>
#include <CL/cl.hpp>
#include <streambuf>
#include <sstream>
#include <cstdio>
#include <chrono>
#include <unistd.h>
#include <dbg.h>

#define SIZE_X 256
#define SIZE_Y 256
#define SIZE_Z 256

#define WORLD_X_START 0.0
#define WORLD_X_END 0.01

#define WORLD_Y_START 0.0
#define WORLD_Y_END 0.01

#define WORLD_Z_START 0.0
#define WORLD_Z_END 0.01

const double WORLD_X_SCALE = (WORLD_X_END-WORLD_X_START)/SIZE_X;
const double WORLD_Y_SCALE = (WORLD_Y_END-WORLD_Y_START)/SIZE_Y;
const double WORLD_Z_SCALE = (WORLD_Z_END-WORLD_Z_START)/SIZE_Z;

#define OPENGL_WORLD_SCALE 1

const int SIZE_XY = (SIZE_X*SIZE_Y);
const int SIZE_XYZ = (SIZE_X*SIZE_Y*SIZE_Z);

extern cl::Program program;
extern cl::CommandQueue queue;
extern cl::Context context;
extern cl::Device default_device;

int idx(int x, int y, int z, int x_len, int y_len);
void init_OpenCL();
void display_array(float potentials[],int s_x, int s_y, int s_z, int z_slice);
void kernel_queue_wrapper(cl::Kernel kernel, cl::NDRange range);
