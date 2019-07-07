#include <vector>
#include <math.h>
#include <iostream>
#include <algorithm> // for copy
#include <iterator> // for ostream_iterator
#include <algorithm>
#include <cmath>

#include <fstream>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <chrono>
#include <vector>
#include "data_structure.hpp"

#define X 0
#define Y 1
#define Z 2

#define X1 0
#define X2 1
#define Y1 2
#define Y2 3
#define Z1 4
#define Z2 5


#define idx(x,y,z,x_len,y_len) ((x_len*y_len*z) + (x_len*y) + x)
#define root_idx(x,y,z,mesh_geometry) ((mesh_geometry.root_x_len*mesh_geometry.root_y_len*z) + (mesh_geometry.root_x_len*y) + x)


template<typename T>
int submesh_side_length(std::vector<T> &input_vector);

//
// void import_mesh(const char* filename, std::vector<bool> &mesh_present, int mesh_geometry[3], float mesh_scale[3], double bounds[6]);

// template<typename T>
// float get_mesh_value(int v_x, int v_y, int v_z, std::vector<std::vector<T>> &mesh, root_mesh_geometry mesh_geometry);
//
// template<typename T>
// void set_mesh_value(float val, int v_x, int v_y, int v_z, std::vector<std::vector<T>> &mesh, root_mesh_geometry mesh_geometry);

// template<typename T>
// float get_mesh_value_world_point(float x, float y, float z, std::vector<std::vector<T>> &mesh, root_mesh_geometry mesh_geometry);
//
// template<typename T>
// void set_mesh_value_world_point(float val, float x, float y, float z, std::vector<std::vector<T>> &mesh, root_mesh_geometry mesh_geometry);

template<typename T>
void enable_mesh_region(std::vector<std::vector<T>> &input_mesh, float bounds[6], root_mesh_geometry mesh_geometry, int submesh_side_length);

template<typename T>
void coarsen_mesh(std::vector<std::vector<T>> &original, std::vector<std::vector<T>> &coarsened, root_mesh_geometry original_geometry, int scale_divisor);

template<typename T>
void decoarsen_mesh(std::vector<std::vector<T>> &original, std::vector<std::vector<T>> &decoarsened, root_mesh_geometry decoarsened_geometry, int scale_divisor);

template<typename T>
float relative_mesh_value(std::vector<std::vector<T>>& input_mesh, int root, int s_x, int s_y, int s_z, int x_rel, int y_rel, int z_rel, root_mesh_geometry mesh_geometry, bool &valid);

std::vector<std::vector<float>> fast_relax_laplace_potentials(std::vector<std::vector<float>> &potentials_vector,
                                        std::vector<std::vector<int>> &boundaries_vector, root_mesh_geometry mesh_geometry, float tolerance, bool field);

void to_csv(std::vector<std::vector<float>> &original, root_mesh_geometry mesh_geometry);

std::vector<std::vector<float>> gauss_seidel(std::vector<std::vector<float>> &potentials, std::vector<std::vector<int>> &boundaries,
    root_mesh_geometry mesh_geometry, float tolerance, bool field, bool ignore_boundaries);

void v_cycle(std::vector<std::vector<float>> &potentials, std::vector<std::vector<int>> &boundaries, root_mesh_geometry mesh_geometry, float tolerance, int i);
