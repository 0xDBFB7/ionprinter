#define SIZE_X 250
#define SIZE_Y 250
#define SIZE_Z 250

#define SIZE_XY (SIZE_X*SIZE_Y)
#define SIZE_XYZ (SIZE_X*SIZE_Y*SIZE_Z)

#pragma OPENCL EXTENSION cl_khr_global_int32_base_atomics : enable

void float_atomic_add(__global float *loc, const float f){
private float old = *loc;
private float sum = old + f;
while(atomic_cmpxchg((__global int*)loc, *((int*)&old), *((int*)&sum)) !=              *((int*)&old)){
    old = *loc;
    sum = old + f;
}
}

inline int idx(int x, int y, int z, int x_len, int y_len){
  return ((x_len*y_len*z) + (x_len*y) + x);
}

void kernel multires_gauss_seidel(global float* potentials, global const int* boundaries, const int res, const int X_SIZE, const int Y_SIZE){
    /* -----------------------------------------------------------------------------
    Call with a 3d NDRange of DIM_SIZE - 2.
    The +1 offset is added to exclude the invalid 0 borders of the mesh; the - 2 handles the other edge.

    ----------------------------------------------------------------------------- */
    // int X_SIZE = (get_global_size(0)*res+(2*res);
    // int Y_SIZE = (get_global_size(1)+res)*res;
    int x = (get_global_id(0)*res)+res;
    int y = (get_global_id(1)*res)+res;
    int z = (get_global_id(2)*res)+res;

    int coord = idx(x,y,z,X_SIZE,Y_SIZE);
    if(!boundaries[coord]){
      potentials[coord] =     (potentials[idx(x+res,y,z,X_SIZE,Y_SIZE)] +
                                  potentials[idx(x-res,y,z,X_SIZE,Y_SIZE)] +
                                  potentials[idx(x,y+res,z,X_SIZE,Y_SIZE)] +
                                  potentials[idx(x,y-res,z,X_SIZE,Y_SIZE)] +
                                  potentials[idx(x,y,z+res,X_SIZE,Y_SIZE)] +
                                  potentials[idx(x,y,z-res,X_SIZE,Y_SIZE)])/6.0;
    }
}


void kernel multires_interpolate(global float* potentials, const int res, const int X_SIZE, const int Y_SIZE){
  /* -----------------------------------------------------------------------------
  http://paulbourke.net/miscellaneous/interpolation/
  ----------------------------------------------------------------------------- */
  int g_x = (get_global_id(0)*res)+res;
  int g_y = (get_global_id(1)*res)+res;
  int g_z = (get_global_id(2)*res)+res;

  float V000 = (potentials[idx(g_x,g_y,g_z,X_SIZE,Y_SIZE)]);
  float V001 = (potentials[idx(g_x,g_y,g_z+res,X_SIZE,Y_SIZE)]);
  float V010 = (potentials[idx(g_x,g_y+res,g_z,X_SIZE,Y_SIZE)]);
  float V100 = (potentials[idx(g_x+res,g_y,g_z,X_SIZE,Y_SIZE)]);
  float V101 = (potentials[idx(g_x+res,g_y,g_z+res,X_SIZE,Y_SIZE)]);
  float V110 = (potentials[idx(g_x+res,g_y+res,g_z,X_SIZE,Y_SIZE)]);
  float V011 = (potentials[idx(g_x,g_y+res,g_z+res,X_SIZE,Y_SIZE)]);
  float V111 = (potentials[idx(g_x+res,g_y+res,g_z+res,X_SIZE,Y_SIZE)]);

  for(int x = 0; x < res; x++){
    for(int y = 0; y < res; y++){
      for(int z = 0; z < res; z++){

        float f_x = (((float)x)/res);
        float f_y = (((float)y)/res);
        float f_z = (((float)z)/res);

        float inverse_x = 1.0-f_x;
        float inverse_y = 1.0-f_y;
        float inverse_z = 1.0-f_z;

        float value = 0;

        value += V000*(inverse_x)*(inverse_y)*(inverse_z);
        value += V100*(f_x)*(inverse_y)*(inverse_z);
        value += V010*(inverse_x)*(f_y)*(inverse_z);
        value += V001*(inverse_x)*(inverse_y)*(f_z);
        value += V101*(f_x)*(inverse_y)*(f_z);
        value += V011*(inverse_x)*(f_y)*(f_z);
        value += V110*(f_x)*(f_y)*(inverse_z);
        value += V111*(f_x)*(f_y)*(f_z);

        potentials[idx(g_x+x,g_y+y,g_z+z,X_SIZE,Y_SIZE)] = value;
      }
    }
  }

}

void kernel subtract(global float* input_1, global float* input_2, global float* output){
  output[get_global_id(0)] = input_1[get_global_id(0)]-input_2[get_global_id(0)];
}

void kernel dist(global float* input_1, global float* input_2, global float* output){
  output[get_global_id(0)] = fabs(input_1[get_global_id(0)]-input_2[get_global_id(0)]);
}
