#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <chrono>
#include <vector>

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

#define SIZE_X 400
#define SIZE_Y 400
#define SIZE_Z 400

const int SIZE_XY = (SIZE_X*SIZE_Y);
const int SIZE_XYZ = (SIZE_X*SIZE_Y*SIZE_Z);

extern cl::Program program;
extern cl::CommandQueue queue;
extern cl::Context context;
extern cl::Device default_device;

void init_OpenCL();
