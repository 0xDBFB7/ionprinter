#include <vector>

int idx(int x, int y, int z, int x_len, int y_len);

int root_idx(int x, int y, int z, root_mesh_geometry mesh_geometry);

float value_plus_z(std::vector<std::vector<float>>& input_mesh, int r_x, int r_y, int r_z,
                                                            int s_x, int s_y, int s_z,
                                                            int sub_len, int root_x_len, int root_y_len, int root_z_len, bool &valid);

float value_minus_z(std::vector<std::vector<float>>& input_mesh, int r_x, int r_y, int r_z,
                                                            int s_x, int s_y, int s_z,
                                                            int sub_len, int root_x_len, int root_y_len, __attribute__((unused)) int root_z_len, bool &valid);

float value_plus_y(std::vector<std::vector<float>>& input_mesh, int r_x, int r_y, int r_z,
                                                            int s_x, int s_y, int s_z,
                                                            int sub_len, int root_x_len, int root_y_len, __attribute__((unused)) int root_z_len, bool &valid);

float value_minus_y(std::vector<std::vector<float>>& input_mesh, int r_x, int r_y, int r_z,
                                                            int s_x, int s_y, int s_z,
                                                            int sub_len, int root_x_len, int root_y_len, __attribute__((unused)) int root_z_len, bool &valid);

float value_plus_x(std::vector<std::vector<float>>& input_mesh, int r_x, int r_y, int r_z,
                                                            int s_x, int s_y, int s_z,
                                                            int sub_len, int root_x_len, int root_y_len, __attribute__((unused)) int root_z_len, bool &valid);

float value_minus_x(std::vector<std::vector<float>>& input_mesh, int r_x, int r_y, int r_z,
                                                            int s_x, int s_y, int s_z,
                                                            int sub_len, int root_x_len, int root_y_len, __attribute__((unused)) int root_z_len, bool &valid);
