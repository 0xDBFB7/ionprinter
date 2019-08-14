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
