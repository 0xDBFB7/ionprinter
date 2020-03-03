#include "physics_mesh.hpp"


// a simple if(ghost_linkages[cell]) over all cells might end up being faster.
__global__ void device_copy_ghost_values_kernel(const physics_mesh &device_struct, float ** values, int depth) {

    int direction = threadIdx.x;

    int this_block = device_struct.block_indices[device_struct.block_depth_lookup[depth]+blockIdx.x];

    int this_cell = this_block + transform_idx(device_struct.mesh_sizes[depth]-1,threadIdx.y,threadIdx.z,
                                                            device_struct.mesh_sizes[depth], direction);

    if(device_struct.ghost_linkages[this_cell]){
        int value_from_indice = device_struct.ghost_linkages[this_cell];
        (*values)[this_cell] = (*values)[value_from_indice];
    }

}

// template <class T>
void physics_mesh::device_copy_ghost_values(physics_mesh * host_struct, physics_mesh * device_struct, float ** values, int depth){
    //here we're using the third dimension as the 'cube facet count'
    //since iterating over facets only requires two dimensions:
    //X and Y.
    //level must be >0.
    //assumes that parameters of the mesh are consistent from host to device.
    //This may not be a safe assumption.

    int num_blocks = (*host_struct).blocks_on_level(depth);
    dim3 threads_per_block(6, (*host_struct).mesh_sizes[depth], (*host_struct).mesh_sizes[depth]);
    //
    device_copy_ghost_values_kernel<<<num_blocks, threads_per_block>>>(*device_struct, values, depth);
    gpu_error_check( cudaPeekAtLastError() );
    gpu_error_check( cudaDeviceSynchronize() );
    //error checking should be done externally; we don't want to cudaDeviceSynchronize after every operation!
}
