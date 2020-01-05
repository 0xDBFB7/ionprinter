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

  state.current_indice = state.block_beginning_indice+idx(state.x,state.y,state.z,mesh_sizes[state.current_depth]);
}

// void update_idx(traverse_state &state, int (&mesh_sizes)[MAX_DEPTH]){
//   //    It seems like this should be better placed in traverse_state:: - however, this would make CUDA integration more difficult.
//
//   state.current_indice = (mesh_sizes[state.current_depth]*mesh_sizes[state.current_depth]*state.z)
//                                                   + (mesh_sizes[state.current_depth]*state.y) + state.x;
// }


void init_state(traverse_state &state, int (&mesh_sizes)[MAX_DEPTH]){
    // pre-compute scales
    float scale = ROOT_WORLD_SCALE;
    // state.world_scale[i] = ROOT_WORLD_SCALE; no! wrong!
    for(int i = 0; i < MAX_DEPTH; i++){
      scale /= mesh_sizes[i];
      state.world_scale[i] = scale;
    }

    //this isn't strictly necessary, but explicit is better than implicit.
    state.current_depth = 0;
    state.block_beginning_indice = 0;
    state.current_indice = 0; //ideally traverse_state would contain a reset_ function,
                              //however I'm not sure that'll be GPU-portable.
    std::fill(state.x_queue, state.x_queue + MAX_DEPTH, 0);
    std::fill(state.y_queue, state.y_queue + MAX_DEPTH, 0);
    std::fill(state.z_queue, state.z_queue + MAX_DEPTH, 0);
    std::fill(state.ref_queue, state.ref_queue + MAX_DEPTH, 0);
    state.x = 0;
    state.y = 0;
    state.z = 0;
    // state.ignore_ghosts = 0;
}

// void unroll_traverse(){
//
// }

bool is_ghost(traverse_state &state, int (&mesh_sizes)[MAX_DEPTH]){
  if(state.x == 0 || state.y == 0 || state.z == 0
      || state.x == mesh_sizes[state.current_depth]
      || state.y == mesh_sizes[state.current_depth]
      || state.z == mesh_sizes[state.current_depth]){
        return true;
  }
  else{
    return false;
  }
}

bool breadth_first(traverse_state &state, int * (refined_indices), int desired_depth, int ignore_ghosts, int (&mesh_sizes)[MAX_DEPTH]){

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

      state.block_beginning_indice = state.ref_queue[state.current_depth];

      if(state.current_depth != desired_depth-1 && refined_indices[state.current_indice]){
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

      if(state.z == mesh_sizes[state.current_depth]-ignore_ghosts){
          //Ascend
          if(state.current_depth == 0){
              return false;
          }

          state.current_depth-=1;
          state.x = state.x_queue[state.current_depth];
          state.y = state.y_queue[state.current_depth];
          state.z = state.z_queue[state.current_depth];

          xyz_traverse(state,mesh_sizes,ignore_ghosts);

          continue;
      }

      break;
    }


    return true;
}



void sync_ghosts(int * array, int * refined_indices, int sync_depth, int (&mesh_sizes)[MAX_DEPTH]){
    // static_assert (sync_depth < MAX_DEPTH, "Assert failed");

    traverse_state state;

    for(init_state(state, mesh_sizes); breadth_first(state, refined_indices, sync_depth, 1, mesh_sizes); xyz_traverse(state, mesh_sizes, 1)){

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

    }
}


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
