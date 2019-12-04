#include <iostream>
#include <omp.h>
#include <chrono>
#include <math.h>
#include <cuda.h>

//export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/arthurdent/Programs/gcc-offload/gcc-offload/install/lib64/
//export CPATH=$CPATH:/usr/local/cuda/include
//export LIBRARY_PATH=$LIBRARY_PATH:/usr/local/cuda/lib64
//~/Programs/gcc-offload/gcc-offload/install/bin/g++ -Wall -Werror -fsanitize=undefined,null -fopenmp -foffload=-lm -fno-fast-math -fno-associative-math openmp_offload.cpp -o ompfl && ./ompfl
//-foffload=-lm -fno-fast-math -fno-associative-math required for math function offload


int main(int argc, char** argv)
{
    const unsigned long int N = 100000000;

    std::cout << "Number of devices: " << omp_get_num_devices() << std::endl;
    std::cout << "Default device: " << omp_get_default_device() << std::endl;

    bool offload;
    #pragma omp target defaultmap(tofrom:scalar)
    offload = !omp_is_initial_device();
    int device = offload ? 0: omp_get_initial_device();

    if (offload) {
      printf("Able to use offloading!\n");
    }

    float * test = new float[N];


    // Allocate
    float* buffer = (float*)omp_target_alloc(N*sizeof(float), device);

    
    // Evaluate
    auto t1 = std::chrono::high_resolution_clock::now();    
    #pragma omp target is_device_ptr(buffer)
    {


    #pragma omp teams distribute parallel for num_threads(1000)
    for(unsigned long int i = 0; i < N; ++i) buffer[i] = sqrtf(i*i*i*i*i*i);
    #pragma omp teams distribute parallel for num_threads(1000)
    for(unsigned long int i = 1; i < N-1; ++i) buffer[i] = buffer[i-1]+buffer[i+1];

        
    }
            
    
    auto t2 = std::chrono::high_resolution_clock::now();

    auto t3 = std::chrono::high_resolution_clock::now();

    for(unsigned long int i = 0; i < N; ++i) test[i] = sqrtf(i*i*i*i*i*i);
    for(unsigned long int i = 1; i < N-1; ++i) test[i] = test[i-1]+test[i+1];
    
    auto t4 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    std::cout << duration << " us, " << "\n";

    
    duration = std::chrono::duration_cast<std::chrono::microseconds>( t4 - t3 ).count();
    std::cout << duration << " us, " << "\n";
    // Cleanup
    omp_target_free(buffer, device);

    delete[] test;

    return 0;
}