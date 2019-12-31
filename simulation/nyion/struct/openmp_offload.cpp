#include <iostream>
#include <omp.h>
#include <chrono>
#include <math.h>
#include <cuda.h>
#include <dbg.h>
//export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/arthurdent/Programs/gcc-offload/gcc-offload/install/lib64/
//export CPATH=$CPATH:/usr/local/cuda/include
//export LIBRARY_PATH=$LIBRARY_PATH:/usr/local/cuda/lib64
//~/Programs/gcc-offload/gcc-offload/install/bin/g++ -Wall -Werror -fsanitize=undefined,null -fopenmp -foffload=-lm -fno-fast-math -fno-associative-math openmp_offload.cpp -o ompfl && ./ompfl
//-foffload=-lm -fno-fast-math -fno-associative-math required for math function offload

struct Point
{
   float * x;
   int y[100];
};

int _idx(int x, int y, int z, int x_len, int y_len){
  return ((x_len*y_len*z) + (x_len*y) + x);
}

int main(int argc, char** argv)
{
    const long int t = 256;
    const unsigned long int N = t*t*t;

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
    float * test2 = new float[N];

    // Allocate
    float* buffer = (float*)omp_target_alloc(N*sizeof(float), device);
    float* buf2 = (float*)omp_target_alloc(N*sizeof(float), device);
    float* p = (float*)omp_target_alloc(N*sizeof(float), device);
    // Evaluate
    auto t1 = std::chrono::high_resolution_clock::now();

    struct Point p1;
    p1.x = p;

    #pragma omp target is_device_ptr(buffer) is_device_ptr(buf2) map(tofrom:p1)
    {

//
    #pragma omp teams distribute
    for(unsigned long int i = 0; i < N-6; ++i) buffer[i] = (buffer[i+1] + buffer[i+2] + buffer[i+3] +buffer[i+4] + buffer[i+5] + buffer[i+5])/ 6.0f;
    #pragma omp teams distribute
    for(int b = 0; b < 1; b++){
        for(int x = 1; x < t-1; x++){
            for(int y = 1; y < t-1; y++){
                for(int z = 1; z < t-1; z++){
                    int idx = (t*t*z) + (t*y) + x;
                    buf2[idx] = (buffer[idx-1]+buffer[idx+1]+buffer[idx-(t*t)]+buffer[idx+(t*t)]+buffer[idx-t]+buffer[idx+t]) / 6.0f;
                }
            }
        }
    }

    p1.x[10] = 10;

    }

    //dbg(p1);

    auto t2 = std::chrono::high_resolution_clock::now();

    for(unsigned long int i = 0; i < N; ++i) test[i] = sqrtf(i*i*i*i*i*i)+56;

    auto t3 = std::chrono::high_resolution_clock::now();

    #pragma omp parallel for
    for(unsigned long int i = 0; i < N-6; ++i) test[i] = (test[i+1]+test[i+2]+test[i+3]+test[i+4]+test[i+5]+test[i+6]) / 6.0;;
//

    for(int b = 0; b < 1; b++){
    #pragma omp parallel for
    for(int x = 1; x < t-1; x++){
        for(int y = 1; y < t-1; y++){
            for(int z = 1; z < t-1; z++){
                int idx = _idx(x,y,z,t,t);
                test2[idx] = (test[idx-1]+test[idx+1]+test[idx-(t*t)]+test[idx+(t*t)]+test[idx-t]+test[idx+t]) / 6.0;
            }
        }
    }
    }

    auto t4 = std::chrono::high_resolution_clock::now();
    dbg(test[_idx(1,1,1,t,t)]);
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()/10.0;
    std::cout << duration << " us, " << "\n";


    duration = std::chrono::duration_cast<std::chrono::microseconds>( t4 - t3 ).count()/10.0;
    std::cout << duration << " us, " << "\n";
    // Cleanup
    omp_target_free(buffer, device);

    delete[] test;

    return 0;
}
