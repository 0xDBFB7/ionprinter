#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include "nyion.hpp"

const int MESH_BUFFER_DEPTH = 3; //includes root
const int MESH_BUFFER_SIZE = (100*100*100)+(1*(100*100*100));
// const int MESH_BUFFER_MAX_BLOCKS = 1000000;
const float ROOT_WORLD_SCALE = 0.1; //meters per root cell

__host__ __device__ __inline__ int cube(int input){
    return input*input*input;
}

struct traverse_state;
struct physics_mesh;

#define named_value(input) std::cout << "    \033[1;33m" << #input << "\033[0m" << " = " << input << "\n";
#define named_array(input,len) std::cout << "    \033[1;33m" << #input << \
                                        "\033[0m [" << len << "]" << " = {"; \
                                        for(uint32_t i = 0; i < len-1; i++){std::cout << input[i] << ",";}; \
                                        if(len){std::cout << input[len-1];} \
                                        std::cout << "}\n";


#define POTENTIAL_TYPE float

//the size of this on the stack must be < 4 KB for reasons of cuda kernel arguments.
struct physics_mesh{

    float world_scale[MESH_BUFFER_DEPTH]; //ROOT_WORLD_SCALE * mesh_scale
    int mesh_sizes[MESH_BUFFER_DEPTH];
    int mesh_depth; //must be enforced because of world_scale issues.

    float * temperature;
    float * potential;
    int32_t * space_charge; //charge probably can't reasonably be fractional - we're not working with quarks?
    uint16_t * boundary_conditions;
    uint32_t * refined_indices;
    uint32_t * ghost_linkages; // can't include ghosts at 'overhangs' - those'll be handled by 'copy_down' I suppose?
                               // - just those on the same level, which'll be changed every iteration.
                               // - could also have 6 pointers to blocks up/down/left/right
                               // I suppose

    uint32_t * block_indices; //an unrolled list of pointers to the beginnings of blocks
                            //needed for fast traversal
                            //must be in ascending order of level - 0,->1,->1,->1,->1,->2,->2,->2,0,0...
    uint32_t block_num[MESH_BUFFER_DEPTH]; //1,4,3,0 //including root
    //we need both block_indices and refined_indices:
    //one provides the spatial data, and one the fast vectorized traverse

    uint32_t buffer_end_pointer;

    physics_mesh(int (&set_mesh_sizes)[MESH_BUFFER_DEPTH], int new_mesh_depth);
    ~physics_mesh();

    void equal(physics_mesh &mesh_2);
    void refine_cell(int current_depth, int current_indice);
    void compute_world_scale();
    void set_ghost_linkages();
    void pretty_print();


    bool breadth_first(traverse_state &state, int start_depth, int end_depth, int ignore_ghosts);


    static void device_constructor(physics_mesh ** device_struct);
    static void copy_to_device(physics_mesh ** device_struct, physics_mesh ** host_struct);
    static void copy_to_host(physics_mesh ** device_struct, physics_mesh ** host_struct);
    static void device_destructor(physics_mesh ** device_struct);
};
//uint_fast32_t probably contraindicated - again, because CUDA.

struct traverse_state{

    int current_depth = 0;
    int block_beginning_indice = 0;
    int current_indice = 0;
    int x_queue[MESH_BUFFER_DEPTH] = {0};
    int y_queue[MESH_BUFFER_DEPTH] = {0};
    int z_queue[MESH_BUFFER_DEPTH] = {0};
    int ref_queue[MESH_BUFFER_DEPTH] = {0};
    int x = 0;
    int y = 0;
    int z = 0;
    bool started_traverse = true;

    traverse_state();

    bool equal(traverse_state &state_2, int depth);
    bool is_ghost(physics_mesh &mesh);
    void pretty_print();
    void cell_world_lookup(physics_mesh &mesh, float &x, float &y, float &z);

};
//Using std::vector would be a good idea. However, this complicates many things with CUDA:
//vect.data() -> pointer, copy to device, then back to struct of vectors? Nah.

void transform_coordinates(uint8_t direction, uint8_t in_x, uint8_t in_y, uint8_t in_z, uint8_t out_x, uint8_t out_y, uint8_t out_z);
    //there are six possible directions in 3d space:
    //+x, +y, +z, -x, -y, -z
    //what about transforms involving the queue? I guess we'll burn that bridge later...




//might be helpful:
// #ifdef __CUDA_ARCH__
//
//    //device code
//
// #else
//
//   //host code
//
// #endif

#endif
