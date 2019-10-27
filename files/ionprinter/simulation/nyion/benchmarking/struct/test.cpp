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



int cell_length(int x_len, int y_len, int z_len){
  /* -----------------------------------------------------------------------------
  Returns cell length in buffer
  //remains to be seen whether ghost points should be implicit or not
  ----------------------------------------------------------------------------- */
  return 	idx((x_len-1),
                (y_len-1),
               	(z_len-1),x_len,y_len);
}



void world_position_offset_from_cell_index(int index, float (&value_world_position)[3], int x_len, int y_len, int z_len){
    value_world_position[X] += ((index % x_len) / ((float) x_len))*WORLD_SIZE_X;
    value_world_position[Y] += ((index / x_len) % y_len) / ((float) y_len))*WORLD_SIZE_Y;
    value_world_position[Z] += ((index / (x_len * y_len)) / ((float) z_len))*WORLD_SIZE_Z;
}


uint32_t add_block(uint32_t &refined_indices, uint8_t &boundaries, float &potential, float &space_charge, uint32_t block_pointer, int depth){
	GLOBAL_CELL_SIZES[]		
	
	for(int cell_index = 0; cell_index < block_; cell_index++){
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



/*

Switched from refbase to zotero primarily because of Zotero's cool chrome extension.
Fought with apache2's WebDAV for hours trying to sync Zotero
I certainly don't begruge Zotero their money, it's just that I would have blown through their plans quite quickly.
wsgidav --host=0.0.0.0 --port=25400 --root ~/NAS/primary_a/share/ --auth=anonymous worked fine, 
Ended up not using DavLock folder like suggested, the defaults work fine.

A set of beam-position buttons can be put at the build platform for calibration
Beam modulation delay can be used to determine distance to objects
The known distance in the emitter can calibrate beam velocity.

E-field derivative

Hey, the bowtie nozzles can totally be built sideways, retaining the liquid metal while
admitting the gas.
Having the build platform at the bottom or side-on would really simplify maintainance
and troubleshooting, since otherwise all the feedthroughs and associated electronics
would be under the machine.
Having the build platform at the bottom would put the parts under compression,
probably better for small cross-sections?
The diffusion pump has to be vertical for condensate drain.

synclient HorizTwoFingerScroll=0
synclient HorizTwoFingerScroll=



------------------
~4% alginate
~94% alumina
~2% gypsum

The powders were added and mixed.
After adding a small amount of water, the concoction became fiberous and
granulated while mixing. More water, and it reached the consistency of play-dough.

Fired with propane, failed - though the part stayed together very well during firing.

Another mix with FeO and TiO2, fired with propane, also failed.

Consider printing graphite. We can't make a graphite wire, so a vapor-phase 
setup would be desirable. The main argument against vapor-phase transport is that
distant nozzles will have a precipitously lower pressure. Perhaps nearer nozzles can be obstructed?

 
------------------

All this is really poor practice, especially since the whole sci-comp community is switching to C++ en masse.
If blocks are of a known length and always added in some definite order,
we could do away with the whole ridiculous pointer-chasing thing and just index 
in some sensible manner.


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
		//group by root cell, assign a cell to each core?
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
		if(depth > 0):
			copy corners (ignoring ghost points) to level below
			copy ghosts to level below 
			interpolate corners to fine
						
		copy ghosts at this depth
		jacobi at this depth
		if refined_indices[]
				jacobi 
				copy ghosts at this dept
	
	
				
	(faster version of refine() that just checks space_charge) 

//////////////////////////////////////
Recursive implementation:


//////////////////////////////////////
Ghost copy operation factor will go like
((n+2)^3 - (n^3)) / (n^3)
Where n is the refined-mesh side length.
n = 10, O = 0.72
n = 20, O = 0.331
n = 50, O = 0.124
/////////////////////////////////////

*/







