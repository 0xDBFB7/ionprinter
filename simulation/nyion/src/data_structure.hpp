#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include "nyion.hpp"

const int MAX_DEPTH = 3;
const int MESH_BUFFER_SIZE = 40;
const int SIZES[MAX_DEPTH] = {8,8,8};


void sync_ghosts(int * array, int * refined_indices, int sync_depth);

template<typename T>
void pretty_print_array(T * input, int start, int end, std::string name);

#define pretty_print_named_array(input, start, end) pretty_print_array(input, start, end, #input)


#endif
