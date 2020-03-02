#include "physics_mesh.hpp"


// a simple if(ghost_linkages[cell]) over all cells might end up being faster.
__global__ void device_copy_ghost_values_kernel(const physics_mesh &device_mesh, float * values, int depth) {

    int direction = threadIdx.x;

    int this_block = block_indices[device_mesh.block_depth_lookup[depth]+blockIdx.x];

    int this_cell = this_block + transform_idx(device_mesh.mesh_sizes[depth]-1,threadIdx.y,threadIdx.z,
                                                            device_mesh.mesh_sizes[depth], direction);

    if(ghost_linkages[this_cell]){
        int value_from_indice = ghost_linkages[this_cell]);
        values[this_cell] = values[value_from_indice];
    }

}


void device_copy_ghost_values(physics_mesh * host_struct, physics_mesh * device_struct, int level){
    //here we're using the third dimension as the 'cube facet count'
    //since iterating over facets only requires two dimensions:
    //X and Y.
    //level must be >0.
    //assumes that parameters of the mesh are consistent from host to device.
    //This may not be a safe assumption.

    int num_blocks = (*host_struct).blocks_on_level(level);
    dim3 threads_per_block(6, (*host_struct).mesh_sizes[level], (*host_struct).mesh_sizes[level]);

    device_copy_ghost_values_kernel<<<num_blocks, threads_per_block>>>(*device_struct);
    gpu_error_check( cudaPeekAtLastError() );
    gpu_error_check( cudaDeviceSynchronize() );
}
