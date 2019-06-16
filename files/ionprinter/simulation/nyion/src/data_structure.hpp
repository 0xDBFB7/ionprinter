#include <vector>


class root_mesh_geometry{
  public:

    root_mesh_geometry(float bounds[6], float root_scale, float sub_scale);

    int root_x_len; //root gridpoints for the mesh
    int root_y_len;
    int root_z_len;
    int root_size; //total gridpoints in root

    float root_scale; //world scale to submesh cube scale

    int sub_len; //sub gridpoints per cube side

    int sub_size; //total gridpoints in sub

    float sub_scale;

    int virtual_x_len; //root_x_len * sub_len; number of gridpoints transparent to other logic
    int virtual_y_len; // just convenience variables for later use
    int virtual_z_len;

    float x_min_bound;
    float x_max_bound;
    float y_min_bound;
    float y_max_bound;
    float z_min_bound;
    float z_max_bound; //f*** getters and setters.

};
