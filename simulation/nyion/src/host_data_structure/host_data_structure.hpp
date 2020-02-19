#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include "nyion.hpp"

const int MESH_BUFFER_DEPTH = 3; //includes root
const int MESH_BUFFER_SIZE = (100*100*100)+(10*(100*100*100));
// const int MESH_BUFFER_MAX_BLOCKS = 1000000;
const float ROOT_WORLD_SCALE = 0.1; //meters per root cell

__host__ __device__ __inline__ int cube(int input){
    return input*input*input;
}

struct traverse_state;
struct physics_mesh;

struct physics_mesh{

    float world_scale[MESH_BUFFER_DEPTH]; //ROOT_WORLD_SCALE * mesh_scale
    int mesh_sizes[MESH_BUFFER_DEPTH];
    int mesh_depth;

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

    physics_mesh(int (&set_mesh_sizes)[MESH_BUFFER_DEPTH], int new_mesh_depth){

        //set scales and sizes
        assert("Increase MESH_BUFFER_DEPTH" && MESH_BUFFER_DEPTH >= new_mesh_depth);
        mesh_depth = new_mesh_depth;
        for(int i = 0; i < MESH_BUFFER_DEPTH; i++){ mesh_sizes[i] = set_mesh_sizes[i]; };

        for(int i = 0; i < MESH_BUFFER_DEPTH; i++){ block_num[i] = 0; };

        // pre-compute scales
        float scale = ROOT_WORLD_SCALE;
        for(int i = 0; i < mesh_depth; i++){
            assert("Mesh size must be > 2" && mesh_sizes[i]-2 > 0);
            scale /= mesh_sizes[i]-2; //-2 compensates for ghost points.
            world_scale[i] = scale;
        }

        //on construction, initialize root
        buffer_end_pointer = cube(mesh_sizes[0]);

        //and allocate memory
        temperature = new float[MESH_BUFFER_SIZE];
        potential = new float[MESH_BUFFER_SIZE];
        space_charge = new int32_t[MESH_BUFFER_SIZE];
        boundary_conditions = new uint16_t[MESH_BUFFER_SIZE];
        refined_indices = new uint32_t[MESH_BUFFER_SIZE];
        ghost_linkages = new uint32_t[MESH_BUFFER_SIZE];
        block_indices = new uint32_t[MESH_BUFFER_SIZE];//max blocks?

        //std::fill not available on GPU.
        for(int i = 0; i < MESH_BUFFER_SIZE; i++){
            temperature[i] = 0;
            potential[i] = 0;
            space_charge[i] = 0; //a canary (perhaps -inf?) might be useful
            boundary_conditions[i] = 0;
            refined_indices[i] = 0;
            ghost_linkages[i] = 0;
            block_indices[i] = 0;
        }
    }

    void copy_to_gpu();
    //first copy struct; constructor never runs on device?
    // Then malloc and memcpy temperature...?
    // create device-only destructor with cudaFree?


    ~physics_mesh(){
        //on destruction,
        delete [] temperature;
        delete [] potential;
        delete [] space_charge;
        delete [] boundary_conditions;
        delete [] refined_indices;
        delete [] ghost_linkages;
        delete [] block_indices;
    }


    void refine_cell(int current_depth, int current_indice);
    bool breadth_first(traverse_state &state, int start_depth, int end_depth, int ignore_ghosts);
    void set_ghost_linkages();
};
//uint_fast32_t probably contraindicated - again, because CUDA.

#define named_value(input) std::cout << "    \033[1;33m" << #input << "\033[0m" << " = " << input << "\n";
#define named_array(input,len) std::cout << "    \033[1;33m" << #input << \
                                        "\033[0m [" << len << "]" << " = {"; \
                                        for(int i = 0; i < len-1; i++){std::cout << input[i] << ",";}; \
                                        if(len){std::cout << input[len-1];} \
                                        std::cout << "}\n";

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

    traverse_state(){
        current_depth = 0;
        block_beginning_indice = 0;
        current_indice = 0;
        started_traverse = true;

        for(int i = 0; i < MESH_BUFFER_DEPTH; i++){
            x_queue[i] = 0;
            y_queue[i] = 0;
            z_queue[i] = 0;
            ref_queue[i] = 0;
        }
        x = 0;
        y = 0;
        z = 0;
    }

    bool equal(traverse_state &state_2, int depth){
        bool e_s = true;

        e_s = e_s && (current_depth == state_2.current_depth);
        e_s = e_s && (block_beginning_indice == state_2.block_beginning_indice);
        e_s = e_s && (current_indice == state_2.current_indice);
        e_s = e_s && (x == state_2.x);
        e_s = e_s && (y == state_2.y);
        e_s = e_s && (z == state_2.z);

        for(int i = 0; i < MESH_BUFFER_DEPTH; i++){
            e_s = e_s && (x_queue[i] == state_2.x_queue[i]);
            e_s = e_s && (y_queue[i] == state_2.y_queue[i]);
            e_s = e_s && (z_queue[i] == state_2.z_queue[i]);
            e_s = e_s && (ref_queue[i] == state_2.ref_queue[i]);
        }

        return e_s;
    }

    bool is_ghost(physics_mesh &mesh);
    void cell_world_lookup(physics_mesh &mesh, float &x, float &y, float &z);
    #ifndef __CUDA_ARCH__
    void pretty_print(){
        std::cout << "\n\033[1;32m traverse_state: \033[0m {\n";

        named_value(current_depth);
        named_value(x);
        named_value(y);
        named_value(z);
        named_value(current_indice);
        named_value(block_beginning_indice);
        named_array(x_queue,MESH_BUFFER_DEPTH);
        named_array(y_queue,MESH_BUFFER_DEPTH);
        named_array(z_queue,MESH_BUFFER_DEPTH);
        named_array(ref_queue,MESH_BUFFER_DEPTH);

        std::cout << "}\n";
    }
    #endif

};
//Using std::vector would be a good idea. However, this complicates many things with CUDA:
//vect.data() -> pointer, copy to device, then back to struct of vectors? Nah.

void transform_coordinates(uint8_t direction, uint8_t in_x, uint8_t in_y, uint8_t in_z, uint8_t out_x, uint8_t out_y, uint8_t out_z){
    //there are six possible directions in 3d space:
    //+x, +y, +z, -x, -y, -z
    //what about transforms involving the queue? I guess we'll burn that bridge later...
}




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
