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

#define X 0
#define Y 1
#define Z 2
#define LEN 3
int GLOBAL_CELL_SIZES[] = {16,16,16,4096,128,128,128,2097152}; //define the dimensions of each refinement depth.
									   //bare linear array because that's easiest to work with in OpenCL.
									   //Should probably go into the fast constant memory on GPU.
									   // in order of depth, X,Y,Z,LEN,X,Y,Z,LEN...
									  
int idx(int x, int y, int z, int x_len, int y_len){
    return ((x_len*y_len*z) + (x_len*y) + x);
}

bool is_inside_boundary(float (&value_world_position)[3]){
  return false;
}



void world_position_offset_from_cell_index(int index, float (&value_world_position)[3], int x_len, int y_len, int z_len){
    value_world_position[X] += ((index % x_len) / ((float) x_len))*WORLD_SIZE_X;
    value_world_position[Y] += ((index / x_len) % y_len) / ((float) y_len))*WORLD_SIZE_Y;
    value_world_position[Z] += ((index / (x_len * y_len)) / ((float) z_len))*WORLD_SIZE_Z;
}


uint32_t add_block(uint32_t &refined_indices, uint8_t &boundaries, float &potential, float &space_charge, uint32_t block_pointer, int depth){
	block_length = GLOBAL_CELL_SIZES[(depth*4)+LEN];
	
	for(int cell_index = 0; cell_index < block_length; cell_index++){
		int index = buffer_start_idx + cell_index; //get buffer index
						
		float world_position[3] = {0};
		world_position_offset_from_cell_index(cell_index, world_position);
		//needs to be relative-ized later
		boundaries[cell_index] = is_inside_boundary(world_position);
		space_charge[cell_index] = 0;
		refined_indices[cell_index] = 0;
		values[cell_index] = 0;		
	}

	return buffer_start_idx + cell_length(size_x,size_y,size_z);
}

uint32_t refine(uint32_t &refined_indices, uint8_t &boundaries, float &potential, float &space_charge, uint32_t block_pointer, int depth){
	int x_length = GLOBAL_CELL_SIZES[(depth*4)+X];
	int y_length = GLOBAL_CELL_SIZES[(depth*4)+Y];
	int z_length = GLOBAL_CELL_SIZES[(depth*4)+Z];
	for(int i = 1; i < x_length-1; i++){ //ignore ghost points; they cannot be refined
		for(int j = 1; j < y_length-1; j++){
			for(int k = 1; k < z_length-1; k++){
				
			}
		}
	}
}

//add_cell(new_cell_position, new_cell_len, refined_indices, boundaries, values, buffer_idx);

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


    uint32_t * refined_indices = new uint32_t[BUFFER_SIZE]; //a series of pointers to corresponding refined cells.
                                                               //Zero if this cell is unrefined; this works because
                                                               //the root cells can't be pointed to.

    uint8_t * boundaries = new uint8_t[BUFFER_SIZE];
    float * potential = new float[BUFFER_SIZE];
	float * space_charge = new float[BUFFER_SIZE]

    int root_cells_x = 100;
    int root_cells_y = 100;

    /* -----------------------------------------------------------------------------

    ----------------------------------------------------------------------------- */


}










