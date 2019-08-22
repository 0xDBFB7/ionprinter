#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <chrono>
#include <vector>
#include <exception>

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

#define SIZE_X 64
#define SIZE_Y 64
#define SIZE_Z 64

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
