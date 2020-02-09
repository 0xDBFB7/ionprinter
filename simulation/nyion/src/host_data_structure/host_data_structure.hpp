#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include "nyion.hpp"

// I don't think it makes much sense to have this variable. It'll complicate all sorts of allocs.
const int MAX_DEPTH = 3;
const int MESH_BUFFER_SIZE = (100*100*100)+(10*(100*100*100));

const float ROOT_WORLD_SCALE = 0.1; //meters per root cell

// Similarly, I can't think of a compelling reason to have non-square arrays.
struct traverse_state{
    float world_scale[MAX_DEPTH]; //ROOT_WORLD_SCALE * mesh_scale
    int max_mesh_depth = MAX_DEPTH;
    int current_depth = 0;
    int block_beginning_indice = 0;
    int current_indice = 0;
    int x_queue[MAX_DEPTH] = {0};
    int y_queue[MAX_DEPTH] = {0};
    int z_queue[MAX_DEPTH] = {0};
    int ref_queue[MAX_DEPTH] = {0};
    int x = 0;
    int y = 0;
    int z = 0;

    traverse_state(int (&mesh_sizes)[MAX_DEPTH], int max_depth){
        assert(MAX_DEPTH >= max_depth);
        // pre-compute scales
        float scale = ROOT_WORLD_SCALE;

        for(int i = 0; i < MAX_DEPTH; i++){
          scale /= mesh_sizes[i]-2; //-2 compensates for ghost points.
          world_scale[i] = scale;
        }

        //this isn't strictly necessary, but explicit is better than implicit.
        current_depth = 0;
        block_beginning_indice = 0;
        current_indice = 0;

        for(int i = 0; i < MAX_DEPTH; i++){
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

struct physics_mesh{

    float * temperature;
    float * potential;
    int32_t * space_charge; //charge probably can't reasonably be fractional - we're not working with quarks?
    uint16_t * boundary_conditions;
    uint32_t * refined_indices;
    uint32_t * ghost_linkages;

    uint32_t buffer_end_pointer;

    physics_mesh(int (&mesh_sizes)[MAX_DEPTH]){
        //on construction, initialize root
        buffer_end_pointer = mesh_sizes[0]*mesh_sizes[0]*mesh_sizes[0];
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
};
//uint_fast32_t probably contraindicated - again, because CUDA.


bool breadth_first(traverse_state &state, int * (refined_indices), int max_depth, int ignore_ghosts, int (&mesh_sizes)[MAX_DEPTH]);
void sync_ghosts(int * array, int * refined_indices, int sync_depth, int (&mesh_sizes)[MAX_DEPTH]);
void init_state(traverse_state &state, int (&mesh_sizes)[MAX_DEPTH]);

__inline__
void xyz_traverse(traverse_state &state, int (&mesh_sizes)[MAX_DEPTH], bool ignore_ghosts){

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

bool is_ghost(traverse_state &state, int (&mesh_sizes)[MAX_DEPTH]);
void update_idx(traverse_state &state, int (&mesh_sizes)[MAX_DEPTH]);
void cell_world_lookup(traverse_state &state, float &x, float &y, float &z);


#endif
