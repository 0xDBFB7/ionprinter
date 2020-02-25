
#include "CUDA_helpers.hpp"

void display_GPU_info(){
    //from https://forge.univ-poitiers.fr/projects/calculateur-hulk/wiki/CUDA_toolkit
    //Thanks y'all!
    const int kb = 1024;
    const int mb = kb * kb;
    std::wcout << "" << std::endl << "=========" << std::endl << std::endl;

    std::wcout << "CUDA version:   v" << CUDART_VERSION << std::endl;

    int devCount;
    cudaGetDeviceCount(&devCount);
    std::wcout << "CUDA Devices: " << std::endl << std::endl;

    for(int i = 0; i < devCount; ++i)
    {
        cudaDeviceProp props;
        cudaGetDeviceProperties(&props, i);
        std::wcout << i << ": " << props.name << ": " << props.major << "." << props.minor << std::endl;
        std::wcout << "  Global memory:   " << props.totalGlobalMem / mb << "mb" << std::endl;
        std::wcout << "  Shared memory:   " << props.sharedMemPerBlock / kb << "kb" << std::endl;
        std::wcout << "  Constant memory: " << props.totalConstMem / kb << "kb" << std::endl;
        std::wcout << "  Block registers: " << props.regsPerBlock << std::endl << std::endl;

        std::wcout << "  Warp size:         " << props.warpSize << std::endl;
        std::wcout << "  Threads per block: " << props.maxThreadsPerBlock << std::endl;
        std::wcout << "  Max block dimensions: [ " << props.maxThreadsDim[0] << ", " << props.maxThreadsDim[1]  << ", " << props.maxThreadsDim[2] << " ]" << std::endl;
        std::wcout << "  Max grid dimensions:  [ " << props.maxGridSize[0] << ", " << props.maxGridSize[1]  << ", " << props.maxGridSize[2] << " ]" << std::endl;
        std::wcout << std::endl;
    }
    std::wcout << "" << std::endl << "=========" << std::endl << std::endl;

}
