#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include "nyion.hpp"

const int MESH_BUFFER_DEPTH = 3;
const int MESH_BUFFER_SIZE = (100*100*100)+(10*(100*100*100));

const float ROOT_WORLD_SCALE = 0.1; //meters per root cell

struct physics_mesh{

    float world_scale[MESH_BUFFER_DEPTH]; //ROOT_WORLD_SCALE * mesh_scale
    int mesh_sizes[MESH_BUFFER_DEPTH];
    int mesh_depth;

    float * temperature;
    float * potential;
    int32_t * space_charge; //charge probably can't reasonably be fractional - we're not working with quarks?
    uint16_t * boundary_conditions;
    uint32_t * refined_indices;
    uint32_t * ghost_linkages;

    uint32_t buffer_end_pointer;

    physics_mesh(int (&set_mesh_sizes)[MESH_BUFFER_DEPTH], int new_mesh_depth){

        //set scales and sizes
        assert("Increase MESH_BUFFER_DEPTH" && MESH_BUFFER_DEPTH >= new_mesh_depth);
        mesh_depth = new_mesh_depth;
        for(int i = 0; i < MESH_BUFFER_SIZE; i++){ mesh_sizes[i] = set_mesh_sizes[i]; };

        // pre-compute scales
        float scale = ROOT_WORLD_SCALE;
        for(int i = 0; i < mesh_depth; i++){
            assert("Mesh size must be > 2" && mesh_sizes[i]-2 > 0);
            scale /= mesh_sizes[i]-2; //-2 compensates for ghost points.
            world_scale[i] = scale;
        }

        //on construction, initialize root
        buffer_end_pointer = mesh_sizes[0]*mesh_sizes[0]*mesh_sizes[0];
        //set size

        //and allocate memory
        temperature = new float[MESH_BUFFER_SIZE];
        potential = new float[MESH_BUFFER_SIZE];
        space_charge = new int32_t[MESH_BUFFER_SIZE];
        boundary_conditions = new uint16_t[MESH_BUFFER_SIZE];
        refined_indices = new uint32_t[MESH_BUFFER_SIZE];
        ghost_linkages = new uint32_t[MESH_BUFFER_SIZE];

        //std::fill not available on GPU.
        for(int i = 0; i < MESH_BUFFER_SIZE; i++){
            temperature[i] = 0;
            potential[i] = 0;
            space_charge[i] = 0; //a canary (perhaps -inf?) might be useful
            boundary_conditions[i] = 0;
            refined_indices[i] = 0;
            ghost_linkages[i] = 0;
        }
    }

    void copy_to_gpu();
    //first copy struct; constructor never runs on device.
    // Then malloc and memcpy temperature...
    // create device-only destructor with cudaFree.


    ~physics_mesh(){
        //on destruction,
        delete [] temperature;
        delete [] potential;
        delete [] space_charge;
        delete [] boundary_conditions;
        delete [] refined_indices;
        delete [] ghost_linkages;
    }

    void refine_cell();
    bool breadth_first(traverse_state &state, int desired_depth, int ignore_ghosts);
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

    traverse_state(){

        //this isn't strictly necessary, but explicit is better than implicit.
        current_depth = 0;
        block_beginning_indice = 0;
        current_indice = 0;

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

};
//Using std::vector would be a good idea. However, this complicates many things with CUDA:
//vect.data() -> pointer, copy to device, then back to struct of vectors? Nah.


__inline__
void xyz_traverse(traverse_state &state, int (&mesh_sizes)[MESH_BUFFER_DEPTH], bool ignore_ghosts){

  //ensure that we don't start in the corner if ghosts are to be ignored.
  if(ignore_ghosts && !state.y){ //slow, stupid
    state.y = 1;
    state.z = 1;
  }

  state.x++;
  if(state.x == mesh_sizes[state.current_depth]-ignore_ghosts) {state.x=ignore_ghosts; state.y++;}
  if(state.y == mesh_sizes[state.current_depth]-ignore_ghosts) {state.y=ignore_ghosts; state.z++;}

  state.x_queue[state.current_depth] = state.x;
  state.y_queue[state.current_depth] = state.y;
  state.z_queue[state.current_depth] = state.z;

  state.current_indice = state.block_beginning_indice+idx(state.x,state.y,state.z,mesh_sizes[state.current_depth]);
}

bool is_ghost(traverse_state &state, int (&mesh_sizes)[MESH_BUFFER_DEPTH]);
void cell_world_lookup(traverse_state &state, float &x, float &y, float &z);


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
