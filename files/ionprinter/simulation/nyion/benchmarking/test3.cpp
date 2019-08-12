#include <iostream>
#include <fstream>
#include <CL/cl.hpp>
#include <streambuf>
#include <sstream>
#include <cstdio>
#include <chrono>

#define SIZE 450

int main(){
    /* -----------------------------------------------------------------------------
    Boilerplate begin
    ----------------------------------------------------------------------------- */
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
    cl::Device default_device=all_devices[0];
    std::cout<< "Using device: "<<default_device.getInfo<CL_DEVICE_NAME>()<<"\n";
    cl::Context context({default_device});
    cl::Program::Sources sources;


    std::fstream kernelFile("kernel.cl");
    std::string content(
      (std::istreambuf_iterator<char>(kernelFile)),
      std::istreambuf_iterator<char>()
      );
    sources.push_back({content.c_str(),content.length()});


    cl::Program program(context,sources);
    if(program.build({default_device})!=CL_SUCCESS){
        std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device)<<"\n";
        exit(1);
    }
    /* -----------------------------------------------------------------------------
    Boilerplate end
    ----------------------------------------------------------------------------- */


    // create buffers on the device
    cl::Buffer buffer_potentials(context,CL_MEM_READ_WRITE,sizeof(double)*(SIZE*SIZE*SIZE));
    cl::Buffer buffer_potentials_out(context,CL_MEM_READ_WRITE,sizeof(double)*(SIZE*SIZE*SIZE));
    cl::Buffer buffer_boundaries(context,CL_MEM_READ_WRITE,sizeof(int)*(SIZE*SIZE*SIZE));


    double * potentials = new double[(SIZE*SIZE*SIZE)];
    double * potentials_out = new double[(SIZE*SIZE*SIZE)];
    int * boundaries = new int[(SIZE*SIZE*SIZE)];

    potentials[(SIZE*SIZE)+5] = 100;
    boundaries[(SIZE*SIZE)+5] = 1;

    //create queue to which we will push commands for the device.
    cl::CommandQueue queue(context,default_device);

    //write arrays A and B to the device
    queue.enqueueWriteBuffer(buffer_potentials,CL_TRUE,0,sizeof(double)*(SIZE*SIZE*SIZE),potentials);
    queue.enqueueWriteBuffer(buffer_boundaries,CL_TRUE,0,sizeof(int)*(SIZE*SIZE*SIZE),boundaries);

    cl::Kernel simple_add(program, "simple_add");
    simple_add.setArg(0, buffer_potentials);
    simple_add.setArg(1, buffer_potentials_out);
    simple_add.setArg(2, buffer_boundaries);
    auto t1 = std::chrono::high_resolution_clock::now();

    queue.enqueueNDRangeKernel(simple_add,cl::NullRange,cl::NDRange((SIZE*SIZE*SIZE)),cl::NullRange);
    queue.finish();
    simple_add.setArg(0, buffer_potentials_out);
    simple_add.setArg(1, buffer_potentials);
    simple_add.setArg(2, buffer_boundaries);
    queue.enqueueNDRangeKernel(simple_add,cl::NullRange,cl::NDRange((SIZE*SIZE*SIZE)),cl::NullRange);
    queue.finish();
    simple_add.setArg(0, buffer_potentials);
    simple_add.setArg(1, buffer_potentials_out);
    simple_add.setArg(2, buffer_boundaries);
    queue.enqueueNDRangeKernel(simple_add,cl::NullRange,cl::NDRange((SIZE*SIZE*SIZE)),cl::NullRange);
    queue.finish();

    auto t2 = std::chrono::high_resolution_clock::now();


    //read result C from the device to array C
    queue.enqueueReadBuffer(buffer_potentials_out,CL_TRUE,0,sizeof(double)*(SIZE*SIZE*SIZE),potentials_out);


    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    std::cout << duration;

    std::cout<<" result: \n";
    for(int i=(SIZE*SIZE);i<(SIZE*SIZE)+10;i++){
        std::cout<<potentials_out[i]<<" ";
    }

    return 0;
}
