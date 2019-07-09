#include <vector>


class root_mesh_geometry{
  public:

    root_mesh_geometry(float bounds[6], float root_scale);

    int root_x_len; //root gridpoints for the mesh
    int root_y_len;
    int root_z_len;

    int root_size; //total gridpoints in root

    float root_scale; //world scale to submesh cube scale

    float x_min_bound;
    float x_max_bound;
    float y_min_bound;
    float y_max_bound;
    float z_min_bound;
    float z_max_bound; //f*** getters and setters.

};
