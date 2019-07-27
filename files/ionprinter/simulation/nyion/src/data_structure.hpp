#include <vector>


class root_mesh_geometry{
  public:

    root_mesh_geometry(double bounds[6], double root_scale);

    int root_x_len; //root gridpoints for the mesh
    int root_y_len;
    int root_z_len;

    int root_size; //total gridpoints in root

    double root_scale; //world scale to submesh cube scale

    double x_min_bound;
    double x_max_bound;
    double y_min_bound;
    double y_max_bound;
    double z_min_bound;
    double z_max_bound; //f*** getters and setters.

};
//
// class macroparticle{
//   public:
//
//     particle(int test);
//
//     double x; //positions
//     double y;
//     double z;
//
//     double v_x; //velocities
//     double v_y;
//     double v_z;
//
//     double mass;
//     double charge;
//
//     double super_factor; //how many real particles this macroparticle represents
//
//     //how do we handle cross-sections?
//     //the following collisions will be modeled:
//     //electron-ion, ion-electron,
//     //
//
// };
