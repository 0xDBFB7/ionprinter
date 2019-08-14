#include "nyion.hpp"


#define SIZE_X 400
#define SIZE_Y 400
#define SIZE_Z 400


const int SIZE_XY = (SIZE_X*SIZE_Y);
const int SIZE_XYZ = (SIZE_X*SIZE_Y*SIZE_Z);

int main(){

    init_OpenCL();

    // create buffers on the device
    cl::Buffer buffer_potentials(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ));
    cl::Buffer buffer_boundaries(context,CL_MEM_READ_WRITE,sizeof(int)*(SIZE_XYZ));


    float * potentials = new float[(SIZE_XYZ)];
    int * boundaries = new int[(SIZE_XYZ)];

    potentials[SIZE_XY+200] = 100;
    boundaries[SIZE_XY+200] = 1;


    //write arrays A and B to the device
    queue.enqueueWriteBuffer(buffer_potentials,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),potentials);
    queue.enqueueWriteBuffer(buffer_boundaries,CL_TRUE,0,sizeof(int)*(SIZE_XYZ),boundaries);

    cl::Kernel gauss_seidel(program, "gauss_seidel");
    gauss_seidel.setArg(0, buffer_potentials);
    gauss_seidel.setArg(1, buffer_boundaries);

    auto t1 = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < 1000; i++){
      queue.enqueueNDRangeKernel(gauss_seidel,cl::NullRange,cl::NDRange((SIZE_XYZ)),cl::NullRange);
      queue.finish();
    }

    auto t2 = std::chrono::high_resolution_clock::now();


    //read result C from the device to array C
    queue.enqueueReadBuffer(buffer_potentials,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),potentials);


    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()/1000.0;
    std::cout << duration;

    std::cout<<" result: \n";
    for(int i=(SIZE_XY)+200;i<(SIZE_XY)+300;i++){
        std::cout<<potentials[i]<<" ";
    }

    return 0;
}
