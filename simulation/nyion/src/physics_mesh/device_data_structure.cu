#include "physics_mesh.hpp"

const int max_cells_per_cuda_block = 1024;

//GPU supports 2^n*65535*65535 blocks of 1024 threads each.
//the mesh exceeds this size significantly.
//so each physical block is broken down further.
//We will assume that a single row of mesh_sizes can never be > 1024
//num_threads_x
void set_GPU_dimensions(physics_mesh * host_struct, dim3 &blocks, dim3 &threads, int depth){
    //number of refined blocks actually present in mesh on this level
    int num_physical_blocks = (*host_struct).blocks_on_level(depth);
    //physical blocks on this level are all the same size,
    int physical_block_width = (*host_struct).mesh_sizes[depth]-2; //ghosts!

    int sub_blocks = max_cells_per_cuda_block/physical_block_width;

    int sub_blocks_y = sub_blocks;
    int sub_blocks_z = 1; //?

    blocks.PHYSICAL_BLOCKS = num_physical_blocks;
    blocks.SUB_BLOCKS_Y = sub_blocks_y;
    blocks.SUB_BLOCKS_Z = sub_blocks_z;

    threads.x = physical_block_width;
    threads.y = physical_block_width/sub_blocks_y;
    threads.z = physical_block_width/sub_blocks_z;

}


//+1 to ignore ghosts
// a simple if(ghost_linkages[cell]) over all cells might end up being faster.
//blockIdx.x is the number of physical blocks in this mesh
//blockIdx y,z are the number of 'imaginary' sub-blocks to fulfil thread requirements
//threadIdx.x is the facet of the cube currently being operated on
//threadIdx y,z are the positions on the facet.
__global__ void device_copy_ghost_values_kernel(physics_mesh &device_struct, float ** values, int depth) {

    int direction = threadIdx.x;

    int this_block = device_struct.block_indices[device_struct.block_depth_lookup[depth]+blockIdx.x];

    int y = ((((device_struct.mesh_sizes[depth]-2)/blockDim.y)*blockIdx.y)+threadIdx.y)+1;
    int z = ((((device_struct.mesh_sizes[depth]-2)/blockDim.z)*blockIdx.z)+threadIdx.z)+1;

    int this_cell = this_block + transform_idx(device_struct.mesh_sizes[depth]-1,y,z,
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
    dim3 threads;
    dim3 blocks;
    //
    // set_GPU_dimensions(host_struct,blocks,threads,depth);
    // threads.x = 6; //override
    //
    // device_copy_ghost_values_kernel<<<blocks, threads>>>(*device_struct, values, depth);
    // gpu_error_check( cudaPeekAtLastError() );
    // gpu_error_check( cudaDeviceSynchronize() );
    //error checking should be done externally; we don't want to cudaDeviceSynchronize after every operation!
}


//+1 to ignore ghosts
__global__ void device_jacobi_relax_kernel(physics_mesh &device_struct, float ** values, int depth) {

    int this_block = device_struct.block_indices[device_struct.block_depth_lookup[depth]+blockIdx.x];

    int this_cell = this_block + idx(threadIdx.x+1,threadIdx.y+1,threadIdx.z+1, device_struct.mesh_sizes[depth]);

    int value_from_indice = device_struct.ghost_linkages[this_cell];
    (*values)[this_cell] = (*values)[value_from_indice];
}


void physics_mesh::device_jacobi_relax(physics_mesh * host_struct, physics_mesh * device_struct, float ** values, int depth){
    int num_blocks = (*host_struct).blocks_on_level(depth);
    dim3 threads_per_block((*host_struct).mesh_sizes[depth]-2, (*host_struct).mesh_sizes[depth]-2, (*host_struct).mesh_sizes[depth]-2); //ignore ghosts

    device_jacobi_relax_kernel<<<num_blocks, threads_per_block>>>(*device_struct, values, depth);
    gpu_error_check( cudaPeekAtLastError() );
    gpu_error_check( cudaDeviceSynchronize() );
}
