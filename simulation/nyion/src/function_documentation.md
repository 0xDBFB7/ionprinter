# A Cell is the small region in a refined mesh. A Block is a set of cells.

struct Mesh{
	float temperature
	float potential
	float space_charge
	uint_16t boundary_conditions
	refined_indices
	ghost_linkages
	
	buffer_end_pointer = 0;
}

//maybe an index function into the struct? What return value? 

solve_electrostatic_field()
