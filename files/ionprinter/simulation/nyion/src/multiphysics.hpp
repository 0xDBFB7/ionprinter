#include <vector>

#define X 0
#define Y 1
#define Z 2

int f_idx(float x, float y, float z, int mesh_geometry[3], float mesh_scale[3]);
int i_idx(int x, int y, int z, int mesh_geometry[3]);
void import_mesh(const char* filename, std::vector<bool> &mesh_present, int mesh_geometry[3], float mesh_scale[3], double bounds[6], float translate[3]);
