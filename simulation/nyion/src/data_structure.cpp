#include "nyion.hpp"
#include "data_structure.hpp"


/*

Implementation of an adaptive block-structured mesh.

Terminology:

Block


*/

struct traverse_state{
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

    void update_idx(int (&mesh_sizes)[MAX_DEPTH]){
      current_indice = (mesh_sizes[current_depth]*mesh_sizes[current_depth]*z) + (mesh_sizes[current_depth]*y) + x;
    }
};

bool breadth_first(traverse_state &state, int * (refined_indices), int max_depth, int ignore_ghosts, int (&mesh_sizes)[MAX_DEPTH]){

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
        state.update_idx(mesh_sizes);
    }
}




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
