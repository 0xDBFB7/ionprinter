#include <iostream>
#include "dbg.h"

const int MAX_DEPTH = 3;
const int MESH_BUFFER_SIZE = 40;
const int SIZES[MAX_DEPTH] = {8,8,8};

int potentials[MESH_BUFFER_SIZE] = {0};
int refined_indices[MESH_BUFFER_SIZE] = {0};


// 
// bool xyz_helper(){
    // 
// }


bool breadth_first(int &buffer_point, int &current_depth, 
                    int (&buffer_ref_queue)[MAX_DEPTH], 
                    int (&buffer_x_queue)[MAX_DEPTH],
                        int &x, int depth, int ignore_ghosts){

    recheck:

    buffer_point = buffer_ref_queue[current_depth];
    
    if(current_depth != depth-1 && refined_indices[buffer_point+x]){
        //Descend
        buffer_x_queue[current_depth] = x;
        current_depth += 1;
        buffer_ref_queue[current_depth] = refined_indices[buffer_point+x];
        x = ignore_ghosts;
        
        goto recheck; // this is hideous! I'm also not sufficiently inventive to figure out a better way.
    }
    
    if(x == SIZES[current_depth]-ignore_ghosts){
        //Ascend
        if(current_depth == 0){
            return false;
        }
        
        current_depth-=1;
        x = buffer_x_queue[current_depth]+1;

        goto recheck;
    }       
    
    return true;
}



void sync_ghosts(int (&array)[MESH_BUFFER_SIZE], int sync_depth){

    int ignore_ghosts = 1;

    int current_depth = 0;
    int buffer_ref_queue[MAX_DEPTH] = {0}; //store previous block reference
    int buffer_x_queue[MAX_DEPTH] = {0};  //store value index in block 
    int buffer_point = 0;    
    int x = ignore_ghosts;

    while(breadth_first(buffer_point, current_depth, 
                    buffer_ref_queue, buffer_x_queue, x, sync_depth, ignore_ghosts)){

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
            }
        }

        x+=1;
    }
}



int main(){

    potentials[14] = 14;
    potentials[17] = 17;
    
        
    refined_indices[1] = 8;
    refined_indices[2] = 16;
    refined_indices[5] = 24;
    refined_indices[21] = 32;
    
    sync_ghosts(potentials,1);      
    dbg(potentials);    
}

