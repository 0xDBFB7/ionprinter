#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include "nyion.hpp"

// I don't think it makes much sense to have this variable. It'll complicate all sorts of allocs.
const int MAX_DEPTH = 3;
const int MESH_BUFFER_SIZE = (100*100*100)+(10*(100*100*100));

const float ROOT_WORLD_SCALE = 0.1; //meters per root cell

// Similarly, I can't think of a compelling reason to have non-square arrays.
//
struct traverse_state{
    float world_scale[MAX_DEPTH]; //ROOT_WORLD_SCALE * mesh_scale
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
    // bool is_ghost = 0;
    // void update_idx(int (&mesh_sizes)[MAX_DEPTH]);
};

bool breadth_first(traverse_state &state, int * (refined_indices), int max_depth, int ignore_ghosts, int (&mesh_sizes)[MAX_DEPTH]);
void sync_ghosts(int * array, int * refined_indices, int sync_depth, int (&mesh_sizes)[MAX_DEPTH]);
void init_state(traverse_state &state, int (&mesh_sizes)[MAX_DEPTH]);
void xyz_traverse(traverse_state &state, int (&mesh_sizes)[MAX_DEPTH], bool ignore_ghosts);
bool is_ghost(traverse_state &state, int (&mesh_sizes)[MAX_DEPTH]);
void update_idx(traverse_state &state, int (&mesh_sizes)[MAX_DEPTH]);
void cell_world_lookup(traverse_state &state, float &x, float &y, float &z, int (&mesh_sizes)[MAX_DEPTH]);


#endif
