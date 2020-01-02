#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include "nyion.hpp"

// I don't think it makes much sense to have this variable. It'll complicate all sorts of allocs.
const int MAX_DEPTH = 3;
const int MESH_BUFFER_SIZE = 40;

// Similarly, I can't think of a compelling reason to have non-square arrays.
//



void sync_ghosts(int * array, int * refined_indices, int sync_depth, int (&mesh_sizes)[MAX_DEPTH]);

#endif
