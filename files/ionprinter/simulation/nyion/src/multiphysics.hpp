

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

#include "visualize.hpp"

#ifndef MULTIPHYSICS_H
#define MULTIPHYSICS_H


#define X 0
#define Y 1
#define Z 2

#define X1 0
#define X2 1
#define Y1 2
#define Y2 3
#define Z1 4
#define Z2 5


// #define idx(x,y,z,x_len,y_len) ((x_len*y_len*z) + (x_len*y) + x)
// #define root_idx(x,y,z,mesh_geometry) ((mesh_geometry.root_x_len*mesh_geometry.root_y_len*z) + (mesh_geometry.root_x_len*y) + x)



int submesh_side_length(std::vector<double> &input_vector);

//
// void import_mesh(const char* filename, std::vector<bool> &mesh_present, int mesh_geometry[3], double mesh_scale[3], double bounds[6]);

// template<typename T>
// double get_mesh_value(int v_x, int v_y, int v_z, std::vector<std::vector<T>> &mesh, root_mesh_geometry mesh_geometry);
//
// template<typename T>
// void set_mesh_value(double val, int v_x, int v_y, int v_z, std::vector<std::vector<T>> &mesh, root_mesh_geometry mesh_geometry);

// template<typename T>
// double get_mesh_value_world_point(double x, double y, double z, std::vector<std::vector<T>> &mesh, root_mesh_geometry mesh_geometry);
//
// template<typename T>
// void set_mesh_value_world_point(double val, double x, double y, double z, std::vector<std::vector<T>> &mesh, root_mesh_geometry mesh_geometry);

void enable_mesh_region(std::vector<std::vector<double>> &input_mesh, double bounds[6], root_mesh_geometry mesh_geometry, int submesh_side_length);

void coarsen_mesh(std::vector<std::vector<double>> &original, std::vector<std::vector<double>> &coarsened, root_mesh_geometry original_geometry, int scale_divisor);

void decoarsen_mesh(std::vector<std::vector<double>> &original, std::vector<std::vector<double>> &decoarsened, root_mesh_geometry decoarsened_geometry, int scale_divisor);

double relative_mesh_value(std::vector<std::vector<double>>& input_mesh, int root, int s_x, int s_y, int s_z, int x_rel, int y_rel, int z_rel, root_mesh_geometry mesh_geometry, bool &valid);


std::vector<std::vector<double>> fast_relax_laplace_potentials(std::vector<std::vector<double>> &potentials_vector,
                                        std::vector<std::vector<double>> &boundaries_vector, root_mesh_geometry mesh_geometry, double tolerance, bool field);


std::vector<std::vector<double>> gauss_seidel(std::vector<std::vector<double>> &potentials, std::vector<std::vector<int>> &boundaries,
    root_mesh_geometry mesh_geometry, double tolerance, bool field, bool ignore_boundaries);

void v_cycle(std::vector<std::vector<double>> &potentials, std::vector<std::vector<int>> &boundaries, root_mesh_geometry mesh_geometry, double tolerance, int i);

double mesh_min(std::vector<std::vector<double>> &input_mesh, root_mesh_geometry mesh_geometry);
double mesh_max(std::vector<std::vector<double>> &input_mesh, root_mesh_geometry mesh_geometry);

#endif
