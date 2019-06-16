#include <vector>
#include <math.h>
#include <iostream>
#include <algorithm> // for copy
#include <iterator> // for ostream_iterator
#include <algorithm>
#include <cmath>
#include <cmath>

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
#define X_LEN 6 //using an integer count takes a few more cycles:
#define Y_LEN 7 //bounds/len will have to be computed every time the scale is required.
#define Z_LEN 8 //not sure if that's a big deal.

int f_idx(float x, float y, float z, int mesh_geometry[3], float mesh_scale[3]);
int i_idx(int x, int y, int z, int mesh_geometry[3]);
int idx_from_position(int x, int y, int z, int x_len, int y_len, int z_len);
void import_mesh(const char* filename, std::vector<bool> &mesh_present, int mesh_geometry[3], float mesh_scale[3], double bounds[6]);

float get_mesh_value(int v_x, int v_y, int v_z, std::vector<std::vector<float>> &mesh, root_mesh_geometry mesh_geometry);

int relax_laplace_potentials(std::vector<std::vector<float>> &potentials, std::vector<std::vector<int>> &boundary_conditions, root_mesh_geometry mesh_geometry, float tolerance);

template<typename T>
void enable_mesh_region(std::vector<std::vector<T>> &mesh, float bounds[6], root_mesh_geometry mesh_geometry);
