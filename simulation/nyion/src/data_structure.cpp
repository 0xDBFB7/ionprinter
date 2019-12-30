#include "nyion.hpp"
#include "data_structure.hpp"



/*

Implementation of an adaptive block-structured mesh.

Terminology:

Block


*/

struct traverse_state{
    unsigned int x_queue[MAX_DEPTH] = {0};
    unsigned int y_queue[MAX_DEPTH] = {0};
    unsigned int z_queue[MAX_DEPTH] = {0};
    unsigned int ref_queue[MAX_DEPTH] = {0};
    int x = 0;
    int y = 0;
    int z = 0;
};

bool breadth_first(int &buffer_point, int &current_depth,
                    int (&buffer_ref_queue)[MAX_DEPTH],
                    int (&buffer_x_queue)[MAX_DEPTH],
                    int (&refined_indices)[MESH_BUFFER_SIZE],
                        int &x, int max_depth, int ignore_ghosts){

    /*
    A traverse through all the cells of all the blocks at a specified level.
    'Breadth first' is perhaps a bit misleading; more precisely it's depth first then breadth across
    depth
    Iterates through the x,y,z values of a block,
    ref_queue stores block indices of parent blocks.

    g o o o o o... g
    ^
    buffer_point

    */

    while(true){

      buffer_point = buffer_ref_queue[current_depth];

      if(current_depth != max_depth-1 && refined_indices[buffer_point+x]){
          //Descend
          buffer_x_queue[current_depth] = x;
          current_depth += 1;
          buffer_ref_queue[current_depth] = refined_indices[buffer_point+x];
          x = ignore_ghosts;

          continue;
      }

      if(x == SIZES[current_depth]-ignore_ghosts){
          //Ascend
          if(current_depth == 0){
              return false;
          }

          current_depth-=1;
          x = buffer_x_queue[current_depth]+1;

          continue;
      }

      break;
    }

    return true;
}


void sync_ghosts(int (&array)[MESH_BUFFER_SIZE], int (&refined_indices)[MESH_BUFFER_SIZE], int sync_depth){

    int ignore_ghosts = 1;

    int current_depth = 0;
    int buffer_ref_queue[MAX_DEPTH] = {0}; //store previous block reference
    int buffer_x_queue[MAX_DEPTH] = {0};  //store value index in block
    int buffer_point = 0;
    int x = ignore_ghosts;

    while(breadth_first(buffer_point, current_depth,
                    buffer_ref_queue, buffer_x_queue, refined_indices, x, sync_depth, ignore_ghosts)){

        //std::cout << current_depth << "," << x << "\n";


        if(current_depth == sync_depth-1 && refined_indices[buffer_point+x]){
            // 0 0 0
            // G G G     potentials[refined_indices[buffer_point+x]+0]
            //-^-^-^--
            // 4 2 6     potentials[refined_indices[buffer_point+x-1]+sizes[current_depth+1]-2]

            if(refined_indices[buffer_point+x-1]){ //shouldn't have to worry about buffer overrun, because ghosts are ignored
                                                    //index block before
                //Update bottom ghost points
                array[refined_indices[buffer_point+x]] = array[refined_indices[buffer_point+x-1]+SIZES[current_depth+1]-2];
            }
            else{
                //pass
            }

            if(refined_indices[buffer_point+x+1]){
                //Update top ghost points
                array[refined_indices[buffer_point+x]+SIZES[current_depth]-1] = array[refined_indices[buffer_point+x+1]+1];
            }
            else{
                //pass // Ghosts at the edge are currently ignored; it must be ensured that no E-field lookups occur near the edges.
                // it would probably make sense to set thes
            }

        }

        x+=1;
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
