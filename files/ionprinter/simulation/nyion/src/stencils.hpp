#include <vector>

int idx(int x, int y, int z, int x_len, int y_len);

int root_idx(int x, int y, int z, root_mesh_geometry mesh_geometry);

double relative_value(std::vector<std::vector<double>>& input_mesh,
                                                            int r_x, int r_y, int r_z,
                                                            int s_x, int s_y, int s_z,
                                                            int x_rel, int y_rel, int z_rel,
                                                            int sub_len, int root_x_len, int root_y_len, int root_z_len, bool &valid);
