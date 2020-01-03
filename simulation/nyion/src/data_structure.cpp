#include "nyion.hpp"
#include "data_structure.hpp"


/*

Implementation of an adaptive block-structured mesh.

Terminology:

Block


Block beginning indices could be unrolled for the compute-intensive portions.
array []  {size(^3), indice, size(^3), indice}

Ghost updates, too - run through once in tree mode, establish ghost link indices and record, have GPU update quickly.



*/


void update_idx(traverse_state &state, int (&mesh_sizes)[MAX_DEPTH]){
  //    It seems like this should be better placed in traverse_state:: - however, this would make CUDA integration more difficult.

  state.current_indice = (mesh_sizes[state.current_depth]*mesh_sizes[state.current_depth]*state.z)
                                                  + (mesh_sizes[state.current_depth]*state.y) + state.x;
}
//
//
void init_scales(traverse_state &state, int (&mesh_sizes)[MAX_DEPTH]){
    // pre-compute scales
    float scale = ROOT_WORLD_SCALE;
    // state.world_scale[i] = ROOT_WORLD_SCALE; no! wrong!
    for(int i = 0; i < MAX_DEPTH; i++){
      scale /= mesh_sizes[i];
      state.world_scale[i] = scale;
    }
}

// void unroll_traverse(){
//
// }

bool breadth_first(traverse_state &state, int * (refined_indices), int max_depth, int ignore_ghosts, int (&mesh_sizes)[MAX_DEPTH]){

    /*
    It seems like this should be better placed in traverse_state:: - however, this would make CUDA integration more difficult.

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

      state.block_beginning_indice = state.ref_queue[state.current_depth]; //block_begin

      if(state.current_depth != max_depth-1 && refined_indices[state.current_indice]){
          //Descend
          state.x_queue[state.current_depth] = state.x;
          state.current_depth += 1;
          state.ref_queue[state.current_depth] = refined_indices[state.current_indice];
          state.x = ignore_ghosts;

          continue;
      }

      if(state.x == mesh_sizes[state.current_depth]-ignore_ghosts){
          //Ascend
          if(state.current_depth == 0){
              return false;
          }

          state.current_depth-=1;
          state.x = state.x_queue[state.current_depth]+1;

          continue;
      }

      break;
    }

    return true;
}




void sync_ghosts(int * array, int * refined_indices, int sync_depth, int (&mesh_sizes)[MAX_DEPTH]){
    // static_assert (sync_depth < MAX_DEPTH, "Assert failed");

    traverse_state state;

    while(breadth_first(state, refined_indices, sync_depth, 1, mesh_sizes)){

        //std::cout << current_depth << "," << x << "\n";


        if(state.current_depth == sync_depth-1 && refined_indices[state.current_indice]){
            // 0 0 0
            // G G G     potentials[refined_indices[block_beginning_indice+x]+0]
            //-^-^-^--
            // 4 2 6     potentials[refined_indices[block_beginning_indice+x-1]+sizes[current_depth+1]-2]

            if(refined_indices[state.current_indice-1]){ //shouldn't have to worry about buffer overrun, because ghosts are ignored
                                                    //index block before
                //Update bottom ghost points
                array[refined_indices[state.current_indice]] = array[refined_indices[state.current_indice-1]+mesh_sizes[state.current_depth+1]-2];
            }
            else{
                //pass
            }

            if(refined_indices[state.current_indice+1]){
                //Update top ghost points
                array[refined_indices[state.current_indice]+mesh_sizes[state.current_depth]-1] = array[refined_indices[state.current_indice+1]+1];
            }
            else{
                //pass // Ghosts at the edge are currently ignored; it must be ensured that no E-field lookups occur near the edges.
                // it would probably make sense to set thes
            }

        }

        state.x+=1;
        update_idx(state,mesh_sizes);
    }
}

// void world_cell_lookup(float x, float y, float z, int max_depth){
//   int x_,y_,z_;
//   for(int depth = 0, depth < max_depth; depth++){
//       x_ += x/(mesh_scale[depth]); //remember ghosts!
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
//
// int main(){
//
//     potentials[14] = 14;
//     potentials[17] = 17;
//
//
//     refined_indices[1] = 8;
//     refined_indices[2] = 16;
//     refined_indices[5] = 24;
//     refined_indices[21] = 32;
//
//     sync_ghosts(potentials,1);
//     dbg(potentials);
// }
//















///Particles

//While clunky, this Structure of Arrays method is 4x more performant than
// a more usable AoS due to better cache efficiency.
//https://arxiv.org/pdf/1710.03462.pdf
