#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

const int MAX_DEPTH = 3;
const int MESH_BUFFER_SIZE = 40;
const int SIZES[MAX_DEPTH] = {8,8,8};


void sync_ghosts(int (&array)[MESH_BUFFER_SIZE], int sync_depth);


#endif
