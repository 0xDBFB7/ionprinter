
#include "physics_mesh.hpp"
#include "traverse_state.hpp"


#include <vector>
#include <math.h>
#include <iostream>
#include <algorithm> // for copy
#include <iterator> // for ostream_iterator
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cmath>

/* -----------------------------------------------------------------------------
DISPLAY SETTINGS
----------------------------------------------------------------------------- */
#define OPENGL_SCALE 10

#define OPENGL_3D_WINDOW_X (1920/2)
#define OPENGL_3D_WINDOW_Y 1080


void initialize_opengl();
void update_screen();
// void draw_geometry_outline(root_mesh_geometry mesh_geometry);
void opengl_clear_screen();
void opengl_draw_axis_cross();
void opengl_3d_mode();
void opengl_test_prism();
// void opengl_2d_mode();
//
void draw_mesh(physics_mesh &mesh, __attribute__((unused)) float * array, bool level_splitting);
void draw_cell(physics_mesh &mesh, traverse_state &state, float R, float G, float B, float A, bool solid, bool level_splitting);

bool escape_hit();
//
//
// void opengl_apply_camera_rotation();
// void opengl_switch_to_graph_window();
// void opengl_switch_to_mesh_window();
// void opengl_graph_1d_vector(std::vector<double> &input, const std::string& title, int index);
