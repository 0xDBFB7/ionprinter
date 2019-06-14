#include <vector>



class root_mesh_region{
  public:
    std::vector<submesh_region> meshes;

    std::vector<float> potential_vector;
    std::vector<int> boundary_conditions_vector;

    std::vector<float> resistance_vector;
    std::vector<float> temperature_vector;

    void add_submesh (int,int);
};


class submesh_region { //contains parameters for a single rectangular sub-mesh.
  public:
    int x_len;
    int y_len;
    int z_len;

    float x_world_scale; //real-world lengths
    float y_world_scale;
    float z_world_scale;

    float x_min_bound;
    float x_max_bound;
    float y_min_bound;
    float y_max_bound;
    float z_min_bound;
    float z_max_bound; //f*** getters and setters.
};


void root_mesh_region::add_submesh(int x_num, int y_num, int z_num, float bounds[6]) {
  /*
  Creates a new submesh with given dimensions.

  the bounds[] array is more convenient for use with vtk.
  */

  submesh_region = new submesh_region();

  x_world_scale = (bounds[X2]-bounds[X1])/x_num;
  y_world_scale = (bounds[X2]-bounds[X1])/x_num;
  z_world_scale = (bounds[X2]-bounds[X1])/x_num;

  x_min_bound = bounds[X1];
  x_max_bound = bounds[X2];
  y_min_bound = bounds[Y1];
  y_max_bound = bounds[Y2];
  z_min_bound = bounds[Z1];
  z_max_bound = bounds[Z2];

}


void root_mesh_region::build_coarse_meshes (int x, int y) {
  width = x;
  height = y;
}
