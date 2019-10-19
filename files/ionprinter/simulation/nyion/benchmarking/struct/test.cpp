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

bool is_inside_boundary(float &value_world_position[3]){
  return false;
}

// uint32_t get_cell_idx(float x, float y, float z, ){
//
// }

int cell_length(int x_len, int y_len, int z_len){
  /* -----------------------------------------------------------------------------
  Returns cell length in buffer
  //remains to be seen whether ghost points should be implicit or not
  ----------------------------------------------------------------------------- */
  return PREAMBLE_SIZE + idx((x_len-1),
                              (y_len-1),
                              (z_len-1),x_len,y_len);
}

//preamble format: world origin, world res x,y,z

void world_position_offset_from_cell_index(int index, float &value_world_position[3], int x_len, int y_len){
    value_world_position[X] += ((index % x_len) / ((float) x_len))*WORLD_SIZE_X;
    value_world_position[Y] += ((index / x_len) % y_len) / ((float) y_len))*WORLD_SIZE_Y;
    value_world_position[Z] += ((index / (x_len * y_len)) / ((float) z_len))*WORLD_SIZE_Z;
}

uint32_t add_cell(int x_len, int y_len, int z_len, uint32_t &refined_indices, uint8_t &boundaries, float &values, uint32_t buffer_start_idx){
  int cell_len = cell_length();
  for(int cell_index = 0; cell_index < cell_len; cell_index++){
    int index = buffer_start_idx + cell_index; //get buffer index

    float world_position[3] = {0};
    world_position_offset_from_cell_index(cell_index, world_position);
    //needs to be relative-ized later
    boundaries[index] = is_inside_boundary(world_position);

    refined_indices[index] = 0;
    values[index] = 0;
  }

  return buffer_start_idx + cell_length(size_x,size_y,size_z);
}



add_cell(new_cell_position, new_cell_len, refined_indices, boundaries, values, buffer_idx);

//if not all 8 corners are homogenous boundaries or vacuum, refine cell.
//
// uint32_t add_cell(float x, float y, float z, int level, uint32_t &refined_indices, uint8_t &boundaries, float &values, int size_x, int size_y, int size_z, uint32_t buffer_idx){
//     /* -----------------------------------------------------------------------------
//     Refines value at level, x,y,z by adding cell.
//     Buffer pointer refers to the point at which new cells should be added.
//     This is a type of linked list.
//     ----------------------------------------------------------------------------- */
//     cell_len = cell_length();
//
//
//     for(int depth = 0; depth < cell_len; depth++){ //recursively traverse cells
//
//     }
//
//     /* -----------------------------------------------------------------------------
//     Each cell fills the entire space of the
//     ----------------------------------------------------------------------------- */
//     for(int value_idx = 0; value_idx < cell_len; value_idx++){
//       int index =
//       boundaries[value_idx] = is_inside_boundary();
//       values[value_idx] = ;
//     }
//
//     return buffer_idx;
// }

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








/*

Functions:



-------particle-------

Electric field:
uint32_t pointer = get_mesh_at_position(x,y,z,finest level);
compute_electric_field(values,pointer){
	values[pointer + PREAMBLE_WORLD_SCALE_X]
}


Could particles store the index of the relevant mesh?

*/



