/*

Having a tree relationship between with levels and blocks makes some kind of sense.
Everything multigrid is naturally tree-based,
having a structure to traverse is great for cell-world and world-cell lookups,
and to generate the linkages between ghosts.
It's also easier to code the construction of meshes.

However, traversing the tree takes jumps and is generally poorly optimized,
especially on a GPU, where a jump stalls the warp.
Once the trees are established on the heap, however, a simple list of indices
can be constructed to traverse linearly.

It might be possible to do away with the tree entirely, but
trying to visualize 'neighbors' with arbitrarily placed blocks
does my head in.

It's important to note that this entire implementation completely disregards
the cache altogether. The indice system means the next block could be halfway to Manitoba
out of cache; there's no cache concurrency, congruency, or congealency; the
&mesh argument means we're probably bringing 20x as much data along as we need for each function;
etc.

If one were truly smart, one could probably eek the same performance out of a
homogenous computing system. I am not.

//block_list
//block_sizes
//connections

//
//Ryzen 7: 0.25 TF/40 gbps. https://en.wikichip.org/wiki/amd/ryzen_7/1700
//GTX 1060: 3 TFlops/160 GBps.
//RTX 2070 Super: 8.2 TF/448.0 GBps - 21 to 51 TFlops half precision.
//V100 hits 14TF/900 GBps + 100TF half.
//https://en.wikipedia.org/wiki/List_of_Nvidia_graphics_processing_units
//
//Special thanks to http://ianfinlayson.net/class/cpsc425/notes/cuda-random!
//
*/

//if all the pointers were put to the end of physics_mesh, perhaps we could simplify the pointer wipe code?
//cudaMemcpy(...,&(struct) -  &(struct->canary)...)
//
// oh, no, structure order doesn't seem to be preserved in the case of

#include "device_data_structure.hpp"
#include "host_data_structure.hpp"


#define CONSTRUCTOR_MACRO(TYPE, NAME, N)     \
    TYPE * device_temp;         \
    /* malloc an array on the device, */ \
    gpu_error_check(cudaMalloc(&device_temp, (N)*sizeof(* device_temp))); \
    /* memset the whole array, not just a buffer_end_pointer's worth */ \
    gpu_error_check(cudaMemset(device_temp,0,(N)*sizeof(* device_temp))); \
    /* then copy the pointer to the new array into the struct. */ \
    gpu_error_check(cudaMemcpy(&((**device_struct).NAME), &device_temp, sizeof((**device_struct).NAME), cudaMemcpyHostToDevice));


void physics_mesh::device_constructor(physics_mesh ** device_struct){
    //construct the struct itself
    gpu_error_check(cudaMalloc(device_struct, sizeof(physics_mesh)));

    //construct the arrays and copy to the GPU
    CONSTRUCTOR_MACRO(float, potential, MESH_BUFFER_SIZE);
}




#define COPY_ARRAY_TO_DEVICE_MACRO(TYPE, NAME, N)     \
    TYPE * NAME; \
    /* get the pointer from the device */ \
    gpu_error_check(cudaMemcpy(&NAME, &((**device_struct).NAME), sizeof(((**device_struct).NAME)), cudaMemcpyDeviceToHost)); \
    /* and now copy the data. */ \
    gpu_error_check(cudaMemcpy(NAME, (**host_struct).NAME,  (N)*sizeof(* NAME), cudaMemcpyHostToDevice));


#define COPY_POINTERS_BACK_TO_DEVICE(NAME)     \
    gpu_error_check(cudaMemcpy(&((**device_struct).NAME), &NAME, sizeof((**device_struct).NAME), cudaMemcpyHostToDevice));


void physics_mesh::copy_to_device(physics_mesh ** device_struct, physics_mesh ** host_struct){
    //double pointer required to preserve malloc edit
    uint32_t length = (**host_struct).buffer_end_pointer;

    //copy array and also save a copy of each array pointer for later
    COPY_ARRAY_TO_DEVICE_MACRO(POTENTIAL_TYPE, potential, length);

    //copy struct itself, wiping all the pointers,
    gpu_error_check(cudaMemcpy(*device_struct, *host_struct, sizeof(physics_mesh), cudaMemcpyHostToDevice));

    //then re-copy the pointers.
    COPY_POINTERS_BACK_TO_DEVICE(potential);

}
//There's a PCIe latency thing here, since we're going * -> host, data -> device,
//but whatever!




#define COPY_ARRAY_TO_HOST_MACRO(TYPE, NAME, N) \
    TYPE * NAME_temp;    \
    /* copy the pointer to the data */  \
    gpu_error_check(cudaMemcpy(&NAME_temp, &((**device_struct).NAME), sizeof(((**device_struct).NAME)), cudaMemcpyDeviceToHost)); \
    /*  then the data itself */ \
    gpu_error_check(cudaMemcpy((**host_struct).NAME, NAME_temp, N*sizeof(* NAME_temp), cudaMemcpyDeviceToHost));



void physics_mesh::copy_to_host(physics_mesh ** device_struct, physics_mesh ** host_struct){

    //save host array pointers for after the wipe
    float * host_temp = (**host_struct).potential;

    //copy struct itself, wiping all the host pointers to arrays,
    gpu_error_check(cudaMemcpy(*host_struct, *device_struct, sizeof(physics_mesh), cudaMemcpyDeviceToHost));

    //then restore host pointers to host arrays
    (**host_struct).potential = host_temp;

    COPY_ARRAY_TO_HOST_MACRO(POTENTIAL_TYPE, potential, ((**host_struct).buffer_end_pointer));

}



#define DEFENESTRATE_ARRAY(TYPE, NAME) \
    TYPE * NAME; \
    /* //copy the pointer to the data */ \
    gpu_error_check(cudaMemcpy(&NAME, &((**device_struct).NAME), sizeof(((**device_struct).NAME)), cudaMemcpyDeviceToHost)); \
    /* //then destroy it! */ \
    gpu_error_check(cudaFree(NAME)); \

void physics_mesh::device_destructor(physics_mesh ** device_struct){

    DEFENESTRATE_ARRAY(POTENTIAL_TYPE, potential);

    //then destroy the struct itself.
    gpu_error_check(cudaFree(&(**device_struct)));
}



void test_cuda(float * x){
}
