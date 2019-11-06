

Potentials 

l0
0 0 0
0 1 0
0 0 0

[0 0 0 0 1 0 0 0 0] 



/*


All this is really poor practice, especially since the whole sci-comp community is switching to C++ en masse.
If blocks are of a known length and always added in some definite order,
we could do away with the whole ridiculous pointer-chasing thing and just index 
in some sensible manner.

We can't pass a struct with all the arrays because OpenCL doesn't support that. CUDA does, but I would not like to switch at this point.
The maximum size of a single malloc is limited. This means seperate arrays. It's ugly and cludgy, but whatever.

Workflow:

initialize_block(block_pointer: 0, size at depth 0)
Initialize root block at buffer[0]; zero all values.  
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
