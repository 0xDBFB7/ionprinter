
#include "physics_mesh.hpp"

struct traverse_state{

    int current_depth = 0;
    int block_beginning_indice = 0;
    int current_indice = 0;
    int x_queue[MESH_BUFFER_DEPTH] = {0};
    int y_queue[MESH_BUFFER_DEPTH] = {0};
    int z_queue[MESH_BUFFER_DEPTH] = {0};
    int ref_queue[MESH_BUFFER_DEPTH] = {0};
    int x = 0;
    int y = 0;
    int z = 0;
    bool started_traverse = true;

    traverse_state();

    bool equal(traverse_state &state_2, int depth);
    bool is_ghost(physics_mesh &mesh);
    void pretty_print();
    void cell_world_lookup(physics_mesh &mesh, float &x, float &y, float &z);

};
