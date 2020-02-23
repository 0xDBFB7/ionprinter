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
// oh, no, structure order doesn't seem to be preserved in some cases.

#include "physics_mesh.hpp"
#include "struct_transfer_helpers.hpp"

void physics_mesh::device_constructor(physics_mesh ** device_struct){
    //construct the struct itself
    gpu_error_check(cudaMalloc(device_struct, sizeof(physics_mesh)));
    //zero the struct memory - just in case
    gpu_error_check(cudaMemset((*device_struct),0,sizeof(physics_mesh)));

    //construct the arrays and copy to the GPU
    constructificate_array_on_struct(&((**device_struct).temperature), MESH_BUFFER_SIZE);
    constructificate_array_on_struct(&((**device_struct).potential), MESH_BUFFER_SIZE);
    constructificate_array_on_struct(&((**device_struct).space_charge), MESH_BUFFER_SIZE);
    constructificate_array_on_struct(&((**device_struct).boundary_conditions), MESH_BUFFER_SIZE);
    constructificate_array_on_struct(&((**device_struct).refined_indices), MESH_BUFFER_SIZE);
    constructificate_array_on_struct(&((**device_struct).ghost_linkages), MESH_BUFFER_SIZE);
    constructificate_array_on_struct(&((**device_struct).block_indices), MESH_BUFFER_SIZE);

}

void physics_mesh::copy_to_device(physics_mesh ** device_struct, physics_mesh ** host_struct){
    //double pointer required to preserve malloc edit
    uint32_t length = (**host_struct).buffer_end_pointer; //length from host

    //copy array and save a copy of each array pointer for later
    COPY_ARRAY_TO_DEVICE(float, temperature, length);
    COPY_ARRAY_TO_DEVICE(float, potential, length);
    COPY_ARRAY_TO_DEVICE(int32_t, space_charge, length);
    COPY_ARRAY_TO_DEVICE(uint16_t, boundary_conditions, length);
    COPY_ARRAY_TO_DEVICE(uint32_t, refined_indices, length);
    COPY_ARRAY_TO_DEVICE(uint32_t, ghost_linkages, length);
    COPY_ARRAY_TO_DEVICE(uint32_t, block_indices, length);


    //copy struct itself, wiping all the pointers,
    gpu_error_check(cudaMemcpy(*device_struct, *host_struct, sizeof(physics_mesh), cudaMemcpyHostToDevice));

    //then re-copy the pointers.
    COPY_POINTERS_BACK_TO_DEVICE(float, temperature, length);
    COPY_POINTERS_BACK_TO_DEVICE(float, potential, length);
    COPY_POINTERS_BACK_TO_DEVICE(int32_t, space_charge, length);
    COPY_POINTERS_BACK_TO_DEVICE(uint16_t, boundary_conditions, length);
    COPY_POINTERS_BACK_TO_DEVICE(uint32_t, refined_indices, length);
    COPY_POINTERS_BACK_TO_DEVICE(uint32_t, ghost_linkages, length);
    COPY_POINTERS_BACK_TO_DEVICE(uint32_t, block_indices, length);

}
//There's a PCIe latency thing here, since we're going * -> host, data -> device,
//but whatever!


void physics_mesh::copy_to_host(physics_mesh ** device_struct, physics_mesh ** host_struct){

    //save host array pointers for after the wipe
    SAVE_HOST_ARRAY_POINTER(float, temperature, length);
    SAVE_HOST_ARRAY_POINTER(float, potential, length);
    SAVE_HOST_ARRAY_POINTER(int32_t, space_charge, length);
    SAVE_HOST_ARRAY_POINTER(uint16_t, boundary_conditions, length);
    SAVE_HOST_ARRAY_POINTER(uint32_t, refined_indices, length);
    SAVE_HOST_ARRAY_POINTER(uint32_t, ghost_linkages, length);
    SAVE_HOST_ARRAY_POINTER(uint32_t, block_indices, length);

    //copy struct itself, wiping all the host pointers to arrays,
    gpu_error_check(cudaMemcpy(*host_struct, *device_struct, sizeof(physics_mesh), cudaMemcpyDeviceToHost));

    //then restore host pointers to host arrays

    RESTORE_HOST_ARRAY_POINTER(float, temperature, length);
    RESTORE_HOST_ARRAY_POINTER(float, potential, length);
    RESTORE_HOST_ARRAY_POINTER(int32_t, space_charge, length);
    RESTORE_HOST_ARRAY_POINTER(uint16_t, boundary_conditions, length);
    RESTORE_HOST_ARRAY_POINTER(uint32_t, refined_indices, length);
    RESTORE_HOST_ARRAY_POINTER(uint32_t, ghost_linkages, length);
    RESTORE_HOST_ARRAY_POINTER(uint32_t, block_indices, length);

    //length from device was already copied to host via struct
    uint32_t length = ((**host_struct).buffer_end_pointer);

    COPY_ARRAY_TO_HOST(float, temperature, length);
    COPY_ARRAY_TO_HOST(float, potential, length);
    COPY_ARRAY_TO_HOST(int32_t, space_charge, length);
    COPY_ARRAY_TO_HOST(uint16_t, boundary_conditions, length);
    COPY_ARRAY_TO_HOST(uint32_t, refined_indices, length);
    COPY_ARRAY_TO_HOST(uint32_t, ghost_linkages, length);
    COPY_ARRAY_TO_HOST(uint32_t, block_indices, length);

}

void physics_mesh::device_destructor(physics_mesh ** device_struct){

    defenestrate_array_from_struct(&((**device_struct).temperature));
    defenestrate_array_from_struct(&((**device_struct).potential));
    defenestrate_array_from_struct(&((**device_struct).space_charge));
    defenestrate_array_from_struct(&((**device_struct).boundary_conditions));
    defenestrate_array_from_struct(&((**device_struct).refined_indices));
    defenestrate_array_from_struct(&((**device_struct).ghost_linkages));
    defenestrate_array_from_struct(&((**device_struct).block_indices));

    //then destroy the struct itself.
    gpu_error_check(cudaFree(&(**device_struct)));
}
