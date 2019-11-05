#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <chrono>
#include <vector>

#include "nyion.hpp"
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

cl::Program program;
cl::CommandQueue queue;
cl::Context context;
cl::Device default_device;


int idx(int x, int y, int z, int x_len, int y_len){
  return ((x_len*y_len*z) + (x_len*y) + x);
}

void kernel_queue_wrapper(cl::Kernel kernel, cl::NDRange range){
  if(queue.enqueueNDRangeKernel(kernel,cl::NullRange,range,cl::NullRange) != CL_SUCCESS){
    std::cout << "OpenCL Exception occurred in Kernel\n";
    throw std::runtime_error("");
  }
}

void display_array(float potentials[],int s_x, int s_y, int s_z, int z_slice){
  double max = *std::max_element(potentials, potentials+(s_x*s_y*s_z));
  double min = *std::min_element(potentials, potentials+(s_x*s_y*s_z));
  for(int y=0;y<s_y/4;y++){
    for(int x=0;x<s_x/4;x++){
      // std::cout<<potentials[idx(x,y,1,SIZE_X,SIZE_Y)]<<" ";
      int r = 0;
      int g = 0;
      int b = 0;
      if(max-min != 0){
        r = (potentials[idx(x,y,z_slice,s_x,s_y)]/((max-min)/2.0))*255.0 * (float)(potentials[idx(x,y,z_slice,s_x,s_y)] > 0);
        g = (potentials[idx(x,y,z_slice,s_x,s_y)]/((max-min)/2.0))*-255.0 * (float)(potentials[idx(x,y,z_slice,s_x,s_y)] < 0);
        b = 0;
      }
      printf("\x1b[38;2;%i;%i;%im# ",r,g,b);
    }
    std::cout<<"\n";
  }
}

void init_OpenCL(){
  std::vector<cl::Platform> all_platforms;
  cl::Platform::get(&all_platforms);
  if(all_platforms.size()==0){
      std::cout<<" No platforms found. Check OpenCL installation!\n";
      exit(1);
  }

  cl::Platform default_platform=all_platforms[0];
  std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";
  std::vector<cl::Device> all_devices;
  default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
  if(all_devices.size()==0){
      std::cout<<" No devices found. Check OpenCL installation!\n";
      exit(1);
  }
  default_device=all_devices[0];
  std::cout<< "Using device: "<<default_device.getInfo<CL_DEVICE_NAME>()<<"\n";
  context = cl::Context({default_device});
  cl::Program::Sources sources;

  std::fstream kernelFile("/tmp/kernel.cl");
  std::string content(
    (std::istreambuf_iterator<char>(kernelFile)),
    std::istreambuf_iterator<char>()
    );
  sources.push_back({content.c_str(),content.length()});

  program = cl::Program(context,sources);
  if(program.build({default_device})!=CL_SUCCESS){
      std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device)<<"\n";
      exit(1);
  }

  queue = cl::CommandQueue(context,default_device);
}
