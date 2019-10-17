#include <vector>
#include <chrono>
#include <iostream>
#include <omp.h>
#include <cstdint>
#include <dbg.h>

#define PREAMBLE_SIZE 5 // a per-cell preamble for whatever
#define STACK_CANARY -INFINITY // if a stencil runs off the edge due to incompetence,
                              //make everything invalid.

#define MAX_CELL_COUNT 1000

#define WORLD_SIZE_X 0.1
#define WORLD_SIZE_Y 0.1
#define WORLD_SIZE_Z 0.1


int idx(int x, int y, int z, int x_len, int y_len){
    return ((x_len*y_len*z) + (x_len*y) + x);
}

bool is_inside_boundary(float x, float y, float z){
  return false;
}

uint32_t get_cell_idx(float x, float y, float z, ){

}

int cell_length(int size_x, int size_y, int size_z){
  /* -----------------------------------------------------------------------------
  Returns cell length in buffer including ghost rows.
  ----------------------------------------------------------------------------- */
  return PREAMBLE_SIZE + idx((size_x -1 + 2),
                              (size_y -1 + 2),
                              (size_z -1 + 2),size_x+2,size_y+2);
}



uint32_t initialize_root_cell(int size_x, int size_y, int size_z, uint32_t &refined_indices, uint8_t &boundaries, float &values, uint32_t buffer_idx){
  cell_len = cell_length();
  for(int i = 0; i < cell_len; i++){
    int index = buffer_idx + i;
    WORLD_SIZE_X;
    boundaries[index] = is_inside_boundary(x,y,z); //

  }

  return buffer_idx + cell_length(size_x,size_y,size_z);
}

//if not all 8 corners are homogenous boundaries or vacuum, refine cell.

uint32_t add_cell(float x, float y, float z, int level, uint32_t &refined_indices, uint8_t &boundaries, float &values, int size_x, int size_y, int size_z, uint32_t buffer_idx){
    /* -----------------------------------------------------------------------------
    Refines value at level, x,y,z by adding cell.
    Buffer pointer refers to the point at which new cells should be added.
    This is a type of linked list.
    ----------------------------------------------------------------------------- */
    cell_len = cell_length();


    for(int depth = 0; depth < cell_len; depth++){ //recursively traverse cells

    }

    /* -----------------------------------------------------------------------------
    Each cell fills the entire space of the
    ----------------------------------------------------------------------------- */
    for(int value_idx = 0; value_idx < cell_len; value_idx++){
      int index =
      boundaries[value_idx] = is_inside_boundary();
      refined_indices[value_idx] = is_inside_boundary();
      values[value_idx] = ;
    }

    return buffer_idx;
}

int main(){
    /* -----------------------------------------------------------------------------

    ----------------------------------------------------------------------------- */


    uint32_t * refined_indices = new uint32_t[MAX_CELL_COUNT]; //a series of pointers to corresponding refined cells.
                                                               //Zero if this cell is unrefined; this works because
                                                               //the root cells can't be pointed to.

    uint8_t * boundaries = new uint8_t[MAX_CELL_COUNT];
    float * values = new float[MAX_CELL_COUNT];


    int root_cells_x = 100;
    int root_cells_y = 100;

    /* -----------------------------------------------------------------------------

    ----------------------------------------------------------------------------- */


}
