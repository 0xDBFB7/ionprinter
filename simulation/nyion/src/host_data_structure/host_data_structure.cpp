#include "nyion.hpp"
#include "host_data_structure.hpp"


/*

Implementation of an adaptive block-structured mesh.

Terminology:

Block


Block beginning indices could be unrolled for the compute-intensive portions.
array []  {size(^3), indice, size(^3), indice}

Ghost updates, too - run through once in tree mode, establish ghost link indices and record, have GPU update quickly.



*/



bool traverse_state::is_ghost(physics_mesh &mesh){
  if(x == 0 || y == 0 || z == 0
      || x == mesh.mesh_sizes[current_depth]-1
      || y == mesh.mesh_sizes[current_depth]-1
      || z == mesh.mesh_sizes[current_depth]-1){
        return true;
  }
  else{
    return false;
  }
}


bool physics_mesh::breadth_first(traverse_state &state, int start_depth, int end_depth, int ignore_ghosts){

    /*
    A traverse through all the cells of all the blocks at a specified level.
    'Breadth first' is perhaps a bit misleading; more precisely it's depth first then breadth across
    depth
    Iterates through the x,y,z values of a block,
    ref_queue stores block indices of parent blocks.

    g o o o o o... g
    ^
    block_beginning_indice

    */

    while(true){
        if(state.started_traverse){
            if(ignore_ghosts){ //slow, stupid
                state.x = 1;
                state.y = 1;
                state.z = 1;
            }
            else{
                state.x = 0;
                state.y = 0;
                state.z = 0;
            }
            state.started_traverse=false;
        }
        else{
            state.x++;
        }

        //ensure that we don't start in the corner if ghosts are to be ignored.

        if(state.x == (mesh_sizes[state.current_depth]-ignore_ghosts)) {state.x=ignore_ghosts; state.y++;}
        if(state.y == (mesh_sizes[state.current_depth]-ignore_ghosts)) {state.y=ignore_ghosts; state.z++;}

        state.x_queue[state.current_depth] = state.x;
        state.y_queue[state.current_depth] = state.y;
        state.z_queue[state.current_depth] = state.z;

        state.current_indice = state.block_beginning_indice+idx(state.x,state.y,state.z,mesh_sizes[state.current_depth]);

        bool just_visited = 0;

        while(true){

          state.block_beginning_indice = state.ref_queue[state.current_depth];
          state.current_indice = state.block_beginning_indice+idx(state.x,state.y,state.z,mesh_sizes[state.current_depth]);

          if(state.current_depth < end_depth && refined_indices[state.current_indice] && !just_visited){
              //Descend
              state.x_queue[state.current_depth] = state.x;
              state.y_queue[state.current_depth] = state.y;
              state.z_queue[state.current_depth] = state.z;
              state.current_depth += 1;
              state.ref_queue[state.current_depth] = refined_indices[state.current_indice];
              state.x = ignore_ghosts;
              state.y = ignore_ghosts;
              state.z = ignore_ghosts;

              continue;
          }

          if(state.z == (mesh_sizes[state.current_depth]-ignore_ghosts)){
              //Ascend
              if(state.current_depth == 0){
                  return false;
              }

              state.current_depth-=1;
              state.x = state.x_queue[state.current_depth];
              state.y = state.y_queue[state.current_depth];
              state.z = state.z_queue[state.current_depth];

              just_visited = true;

              continue;
          }

          break;
        }

        if(state.current_depth >= start_depth && state.current_depth <= end_depth){
            return true;
        }
        else{
            continue;
        }
    }
}

void physics_mesh::set_ghost_linkages(){


    // for(traverse_state state; breadth_first(state, mesh_depth, 1, true);){

        //std::cout << current_depth << "," << x << "\n";


        // if(state.current_depth == sync_depth-1 && refined_indices[state.current_indice]){
        //     // 0 0 0
        //     // G G G     potentials[refined_indices[block_beginning_indice+x]+0]
        //     //-^-^-^--
        //     // 4 2 6     potentials[refined_indices[block_beginning_indice+x-1]+sizes[current_depth+1]-2]
        //
        //     if(refined_indices[state.current_indice-1]){ //shouldn't have to worry about buffer overrun, because ghosts are ignored
        //                                             //index block before
        //         //Update bottom ghost points
        //         // array[refined_indices[state.current_indice]] = array[refined_indices[state.current_indice-1]+mesh_sizes[state.current_depth+1]-2];
        //     }
        //     else{
        //         //pass
        //     }
        //
        //     if(refined_indices[state.current_indice+1]){
        //         //Update top ghost points
        //         // array[refined_indices[state.current_indice]+mesh_sizes[state.current_depth]-1] = array[refined_indices[state.current_indice+1]+1];
        //     }
        //     else{
        //         //pass // Ghosts at the edge are currently ignored; it must be ensured that no E-field lookups occur near the edges.
        //         // it would probably make sense to set thes
        //     }
        // }
    // }
}

void traverse_state::cell_world_lookup(physics_mesh &mesh, float &x, float &y, float &z){
  x = 0;
  y = 0;
  z = 0;
  for(int i = 0; i < ((current_depth+1)) && (i < MESH_BUFFER_DEPTH); i++){
    x += mesh.world_scale[i]*(x_queue[i]-1); //ghost offset
    y += mesh.world_scale[i]*(y_queue[i]-1);
    z += mesh.world_scale[i]*(z_queue[i]-1);
  }
}


//
// int world_cell_index_lookup(float x, float y, float z, int max_depth){
//   int x_,y_,z_;
//   for(int depth = 0, depth < max_depth; depth++){
//       x_ = (int)floor(x/(mesh_scale[depth]));
//       y_ = (int)floor(y/(mesh_scale[depth]));
//       z_ = (int)floor(z/(mesh_scale[depth])); //remember ghosts!
//
//       x -= x_*mesh_scale[depth]
//   }
// }


/*

 - particle moves

# Direct lookup
 - e field
 - DSMC collisions

# Breadth first

 - copy ghosts
 - jacobi
 - copy down
 - visualization

*/














///Particles

//While clunky, this Structure of Arrays method is 4x more performant than
// a more usable AoS due to better cache efficiency.
//https://arxiv.org/pdf/1710.03462.pdf
