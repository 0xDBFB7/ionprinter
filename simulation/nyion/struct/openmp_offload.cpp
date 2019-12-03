#include <iostream>
#include <omp.h>
#include <chrono>
#include <math.h>
#include <cuda.h>

//export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/arthurdent/Programs/gcc-offload/gcc-offload/install/lib64/
//export CPATH=$CPATH:/usr/local/cuda/include
//export LIBRARY_PATH=$LIBRARY_PATH:/usr/local/cuda/lib64
//~/Programs/gcc-offload/gcc-offload/install/bin/g++ $FLAGS openmp_offload.cpp -o ompfl && ./ompfl
//-foffload=-lm -fno-fast-math -fno-associative-math required for math function offload


int main(int argc, char** argv)
{
    const unsigned long int N = 10000000;

    std::cout << "Number of devices: " << omp_get_num_devices() << std::endl;
    std::cout << "Default device: " << omp_get_default_device() << std::endl;

    bool offload;
    #pragma omp target defaultmap(tofrom:scalar)
    offload = !omp_is_initial_device();
    int device = offload ? 0: omp_get_initial_device();

    if (offload) {
      printf("Able to use offloading!\n");
    }

    int * test = new int[N];


    // Allocate
    float* buffer = (float*)omp_target_alloc(N*sizeof(float), device);

    // Evaluate
    auto t1 = std::chrono::high_resolution_clock::now();    
    #pragma omp target is_device_ptr(buffer)
    {
    
    #pragma omp parallel for
    for(unsigned long int i = 0; i < N; ++i) buffer[i] = sqrtf(i*i*i*i*i*i);
    
    }
    
    //for(unsigned long int i = 0; i < N; ++i) test[i] = sqrtf(i*i*i*i*i*i);

    auto t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    std::cout << duration << " us, " << "\n";
    // Cleanup
    omp_target_free(buffer, device);

    delete[] test;

    return 0;
}