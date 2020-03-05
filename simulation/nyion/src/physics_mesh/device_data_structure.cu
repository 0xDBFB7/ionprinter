#include "device_data_structure.hpp"

const int max_cells_per_cuda_block = 1024;



//GPU supports 2^n*65535*65535 blocks of 1024 threads each.
//it is assumed that one yz plane of each block cannot exceed 1024 cells;
//that is, mesh_sizes cannot be > 32.
void set_GPU_dimensions(physics_mesh * host_struct, dim3 &blocks, dim3 &threads, int depth){
    //number of refined blocks actually present in mesh on this level
    int num_physical_blocks = (*host_struct).blocks_on_level(depth);
    //physical blocks on this level are all the same size,
    int physical_block_width = (*host_struct).mesh_sizes[depth]-2; //ghosts!

    int sub_blocks = ceil(((float)cube(physical_block_width))/((float)max_cells_per_cuda_block));
    while(physical_block_width % sub_blocks != 0){
        //if the block can't be divided evenly in Z
        sub_blocks++;
        //will get to physical_block_width at most.
    }

    blocks.PHYSICAL_BLOCKS = num_physical_blocks;
    blocks.SUB_BLOCKS = sub_blocks;

    threads.x = physical_block_width;
    threads.y = physical_block_width;
    threads.z = physical_block_width/sub_blocks;
}


//+1 to ignore ghosts
// a simple if(ghost_linkages[cell]) over all cells might end up being faster.
__global__ void device_copy_ghost_values_kernel(physics_mesh &device_struct, float ** values, int depth) {

    int direction = threadIdx.x;

    int this_block = device_struct.block_indices[device_struct.block_depth_lookup[depth]+blockIdx.PHYSICAL_BLOCKS];

    int x = device_struct.mesh_sizes[depth]-1;
    int y = (threadIdx.y)+1;
    int z = ((((device_struct.mesh_sizes[depth]-2)/gridDim.SUB_BLOCKS)*blockIdx.SUB_BLOCKS)+threadIdx.z)+1;

    int this_cell = this_block + transform_idx(x,y,z, device_struct.mesh_sizes[depth], direction);

    if(device_struct.ghost_linkages[this_cell]){
        int value_from_indice = device_struct.ghost_linkages[this_cell];
        (*values)[this_cell] = (*values)[value_from_indice];
    }
}

// template <class T>
void physics_mesh::device_copy_ghost_values(physics_mesh * host_struct, physics_mesh * device_struct, float ** values, int depth){
    if(depth == 0) return;
    //here we're using the third dimension as the 'cube facet count'
    //since iterating over facets only requires two dimensions:
    //X and Y.
    //level must be >0.
    //assumes that parameters of the mesh are consistent from host to device.
    //This may not be a safe assumption.
    dim3 threads;
    dim3 blocks;

    set_GPU_dimensions(host_struct,blocks,threads,depth);
    threads.x = 6; //override

    device_copy_ghost_values_kernel<<<blocks, threads>>>(*device_struct, values, depth);
    // gpu_error_check( cudaPeekAtLastError() );
    // gpu_error_check( cudaDeviceSynchronize() );
    //error checking now handled externally.
}


//+1 to ignore ghosts
__global__ void device_jacobi_relax_kernel(physics_mesh &device_struct, float ** input, float * output, int depth) {
    //see https://people.eecs.berkeley.edu/~demmel/cs267/lecture24/lecture24.html

    int this_block = device_struct.block_indices[device_struct.block_depth_lookup[depth]+blockIdx.x];

    int x = threadIdx.x+1;
    int y = threadIdx.y+1; //if there's a bug here, make sure gridDim and blockDim are correct.
    int z = ((((device_struct.mesh_sizes[depth]-2)/gridDim.SUB_BLOCKS)*blockIdx.SUB_BLOCKS)+threadIdx.z)+1;

    int this_cell = this_block + idx(x, y, z, device_struct.mesh_sizes[depth]);

    bool boundary_condition_check = (device_struct.boundary_conditions[this_cell] == 0); //1 if cell should be updated

    float sum = 0;
    sum += (*input)[this_block + idx(x+1, y, z, device_struct.mesh_sizes[depth])];
    sum += (*input)[this_block + idx(x-1, y, z, device_struct.mesh_sizes[depth])];
    sum += (*input)[this_block + idx(x, y+1, z, device_struct.mesh_sizes[depth])];
    sum += (*input)[this_block + idx(x, y-1, z, device_struct.mesh_sizes[depth])];
    sum += (*input)[this_block + idx(x, y, z+1, device_struct.mesh_sizes[depth])];
    sum += (*input)[this_block + idx(x, y, z-1, device_struct.mesh_sizes[depth])];
    sum += (device_struct.space_charge[this_cell]/EPSILON_ZERO);

    (output)[this_cell] = ((1-boundary_condition_check)*(*input)[this_cell]) //leave cell unchanged if a boundary is present - saves a jump
                                    + (boundary_condition_check * (sum)/(6.0f));

}

__global__ void device_copy_kernel(physics_mesh &device_struct, float * input, float ** output, int depth) {
    int this_block = device_struct.block_indices[device_struct.block_depth_lookup[depth]+blockIdx.x];

    int x = threadIdx.x+1;
    int y = threadIdx.y+1; //if there's a bug here, make sure gridDim and blockDim are correct.
    int z = ((((device_struct.mesh_sizes[depth]-2)/gridDim.SUB_BLOCKS)*blockIdx.SUB_BLOCKS)+threadIdx.z)+1;

    int this_cell = this_block + idx(x, y, z, device_struct.mesh_sizes[depth]);

    (*output)[this_cell] = (input)[this_cell];
}


void physics_mesh::device_jacobi_relax(physics_mesh * host_struct, physics_mesh * device_struct, float ** values, int iterations, int depth){
    dim3 threads;
    dim3 blocks;

    set_GPU_dimensions(host_struct,blocks,threads,depth);

    float * device_temp;
    //this is basically free since nothing needs to be zeroed
    gpu_error_check(cudaMalloc(&device_temp, sizeof(float)*(*host_struct).buffer_end_pointer));

    physics_mesh::device_copy_ghost_values(host_struct, device_struct, values, depth);

    for(int i = 0; i < iterations; i++){
        device_jacobi_relax_kernel<<<blocks, threads>>>(*device_struct, values, device_temp, depth);

        device_copy_kernel<<<blocks, threads>>>(*device_struct, device_temp, values, depth); //double-buffering adds quite a bit of complexity.

        device_copy_ghost_values(host_struct, device_struct, values, depth);

    }


    //also basically free
    cudaFree(device_temp);

    gpu_error_check( cudaPeekAtLastError() );
    gpu_error_check( cudaDeviceSynchronize() );


}
