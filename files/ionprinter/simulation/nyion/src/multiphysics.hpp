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


#define X 0
#define Y 1
#define Z 2

int f_idx(float x, float y, float z, int mesh_geometry[3], float mesh_scale[3]);
int i_idx(int x, int y, int z, int mesh_geometry[3]);
void import_mesh(const char* filename, std::vector<bool> &mesh_present, int mesh_geometry[3], float mesh_scale[3], double bounds[6]);

void relax_laplace_potentials(std::vector<float> &potentials, std::vector<int> &boundary_conditions, std::vector<bool> &active,
                                                                                              int mesh_geometry[3], float tolerance);
