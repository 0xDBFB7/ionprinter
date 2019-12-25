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

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>


/* -----------------------------------------------------------------------------
DISPLAY SETTINGS
----------------------------------------------------------------------------- */
#define OPENGL_WORLD_SCALE 0.0005 //m/opengl index
#define OPENGL_3D_WINDOW_X 1920
#define OPENGL_3D_WINDOW_Y 1080
#define OPENGL_GRAPH_WINDOW_X 1000
#define OPENGL_GRAPH_WINDOW_Y 1000

#define OPENGL_GRAPH_COUNT 5

#define OPENGL_GRAPH_X_OFFSET 40
#define OPENGL_GRAPH_Y_OFFSET 70

void initialize_opengl(root_mesh_geometry mesh_geometry);
void update_screen();
void draw_geometry_outline(root_mesh_geometry mesh_geometry);
void opengl_clear_screen();
void opengl_draw_axis_cross();
void opengl_3d_mode();
void opengl_2d_mode();

void draw_mesh(std::vector<std::vector<double>>& input_mesh,root_mesh_geometry mesh_geometry);


void opengl_apply_camera_rotation();
void opengl_switch_to_graph_window();
void opengl_switch_to_mesh_window();
void opengl_graph_1d_vector(std::vector<double> &input, const std::string& title, int index);
