#ifndef CONFIG_H
#define CONFIG_H

const int MESH_BUFFER_DEPTH = 4; //includes root
const int MESH_BUFFER_SIZE = (100*100*100)+(1*(100*100*100));
// const int MESH_BUFFER_MAX_BLOCKS = 1000000;
const float ROOT_WORLD_SCALE = 0.1; //meters per root cell


//The GPU can only tolerate 1024 cells per block;
//32^2, or 


//A reasonable worst-case mesh:
//we want 50 micron res. in a 20x20x20cm region
//
#endif
