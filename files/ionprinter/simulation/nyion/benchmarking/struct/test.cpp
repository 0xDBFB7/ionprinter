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

int idx(int x, int y, int z, int x_len, int y_len){
    return ((x_len*y_len*z) + (x_len*y) + x);
}

bool is_inside_boundary(float (&value_world_position)[3]){
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

void world_position_offset_from_cell_index(int index, float (&value_world_position)[3], int x_len, int y_len, int z_len){
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
    float * potential_values = new float[BUFFER_SIZE];
	float * space_charge = new float[BUFFER_SIZE]

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

*/

/*

Workflow:

initialize_block(block_pointer: 0, size at depth 0)
Initialize root block at buffer[0]; zero all values. Set size and length in preamble. Size should be whole xyz because root could be rectangular.
Iterate over root cells (0,end):
	Get cell's world-space position,
	 set potential_values[] and boundaries[].

function refine(block_pointer: 0, depth: 0)
Iterate over root cells (1,end-1 xyz): //remember the ghost points
	xyz = idx(size: block_pointer+preamble)
	If not boundaries[xyz] == [x+1] == [y+1] == [z+1] == [x+1 z+1...] 
		(if there's detail that should be captured, do end check to avoid +1 overrun) OR boundaries[xyz] == marked OR space_charge[] != 0 
	 set refined_indices[this root cell index] = buffer_trailing_index
	 and add new block at buffer_trailing_index
	 initialize_block(block_pointer: buffer_trailing_index, size at this depth)
	 
	if(refined_indices[cell index] != 0):
	
		refine(block_pointer: refined_indices[cell_index], depth: depth+1)
		Iterate over refined_indices[cell index] (1,end-1 xyz):
			...refine() again if depth < max

function copy_ghosts_level()
Iterate over root cells ():
	

(copy buffer to GPU)

while(True):

	Iterate over particles:
		world-space -> root cell idx, xyz
		space_charge[0 + root cell] += particle charge / e0 //has to be atomic! oh noes!
		if refined_indices[0 + root cell idx]
			block idx = refined_indices[0 + root cell idx]
			world-space x -= WORLD_SCALE * root cell idx x... (subtract origin of root cell)
			(world space is now small xyz distance within refined root cell)
			world-space -> cell idx
			space_charge[block idx + cell idx] = ... //each particle should only charge one cell at each depth
			if refined_indices[block_idx + cell idx]:
				...

	Iterate over root values(1,end-1 xyz):
		jacobi
		if refined_indices[]
			copy_down
			interpolate corners 1 in from end
				jacobi
				copy ghosts at this depth?
			...

		
	copy_ghosts()
		
	

			
	(faster version of refine() that just checks space_charge) 
*/







