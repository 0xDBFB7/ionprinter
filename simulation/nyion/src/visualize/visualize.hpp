#include <vector>
#include <math.h>
#include <iostream>
#include <algorithm> // for copy
#include <iterator> // for ostream_iterator
#include <algorithm>
#include <cmath>
#include "host_data_structure.hpp"



#include <fstream>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <chrono>
#include <vector>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/glx.h>


/* -----------------------------------------------------------------------------
DISPLAY SETTINGS
----------------------------------------------------------------------------- */
#define OPENGL_SCALE 10

#define OPENGL_3D_WINDOW_X 1920
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
void draw_mesh(float * array, int * refined_indices, int (&mesh_sizes)[MAX_DEPTH], bool level_splitting);
//
//
// void opengl_apply_camera_rotation();
// void opengl_switch_to_graph_window();
// void opengl_switch_to_mesh_window();
// void opengl_graph_1d_vector(std::vector<double> &input, const std::string& title, int index);
