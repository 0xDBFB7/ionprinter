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

#define MESH_SIZE_X 0.1
#define MESH_SIZE_Y 0.1
#define MESH_SIZE_Z 0.1


int idx(int x, int y, int z, int x_len, int y_len){
    return ((x_len*y_len*z) + (x_len*y) + x);
}

bool is_inside_boundary(){
  return false;
}

uint32_t add_cell(float x, float y, float z, uint32_t &refined_indices, uint8_t &boundaries, float &values, int size_x, int size_y, int size_z, uint32_t heap_idx){
    /* -----------------------------------------------------------------------------
    Refines value at level, x,y,z by adding cell.
    Heap pointer refers to the point at which new cells should be added.
    ----------------------------------------------------------------------------- */
    cell_length = PREAMBLE_SIZE + idx((size_x -1 + 2),
                                      (size_y -1 + 2),
                                      (size_z -1 + 2),size_x,size_y);  //zero indexed, 2 ghost rows


    for(int depth = 0; depth < ; depth++){ //recursively traverse cells

    }

    /* -----------------------------------------------------------------------------
    Each cell fills the entire space of the
    ----------------------------------------------------------------------------- */
    for(int value_idx = 0; value_idx < cell_length; value_idx++){
      int index =
      boundaries[] is_inside_boundary();
    }

    return heap_idx;
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
