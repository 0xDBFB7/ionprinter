
inline int idx(int x, int y, int z, int x_len, int y_len){
  return ((x_len*y_len*z) + (x_len*y) + x);
}

void kernel multires_gauss_seidel(global float* U, global const float* b, int res, const int X_SIZE, const int Y_SIZE){
    /* -----------------------------------------------------------------------------
    Call with a 3d NDRange of DIM_SIZE - 2.
    The +1 offset is added to exclude the invalid 0 borders of the mesh; the - 2 handles the other edge.

    ----------------------------------------------------------------------------- */
    int x = (get_global_id(0)*res)+res;
    int y = (get_global_id(1)*res)+res;
    int z = (get_global_id(2)*res)+res;

    int coord = idx(x,y,z,X_SIZE,Y_SIZE);
    U[coord] = (U[idx(x+res,y,z,X_SIZE,Y_SIZE)] +
                U[idx(x-res,y,z,X_SIZE,Y_SIZE)] +
                U[idx(x,y+res,z,X_SIZE,Y_SIZE)] +
                U[idx(x,y-res,z,X_SIZE,Y_SIZE)] +
                U[idx(x,y,z+res,X_SIZE,Y_SIZE)] +
                U[idx(x,y,z-res,X_SIZE,Y_SIZE)] +
                b[idx(x,y,z,X_SIZE,Y_SIZE)])/6.0;
}
//
// void kernel potentials_gauss_seidel(global float* U, global const int* boundaries, global const float* electrode_potentials, int res, const int X_SIZE, const int Y_SIZE){
//     /* -----------------------------------------------------------------------------
//     Call with a 3d NDRange of DIM_SIZE - 2.
//     The +1 offset is added to exclude the invalid 0 borders of the mesh; the - 2 handles the other edge.
//
//     ----------------------------------------------------------------------------- */
//     int x = (get_global_id(0)*res)+res;
//     int y = (get_global_id(1)*res)+res;
//     int z = (get_global_id(2)*res)+res;
//
//     int coord = idx(x,y,z,X_SIZE,Y_SIZE);
//     float boundary_potential = 0;
//     if(boundaries[coord] > 0){
//       boundary_potential = electrodes[boundaries[coord]];
//     }
//     potentials[coord] =     (potentials[idx(x+res,y,z,X_SIZE,Y_SIZE)] +
//                               potentials[idx(x-res,y,z,X_SIZE,Y_SIZE)] +
//                               potentials[idx(x,y+res,z,X_SIZE,Y_SIZE)] +
//                               potentials[idx(x,y-res,z,X_SIZE,Y_SIZE)] +
//                               potentials[idx(x,y,z+res,X_SIZE,Y_SIZE)] +
//                               potentials[idx(x,y,z-res,X_SIZE,Y_SIZE)] +
//                               boundary_potential)/6.0;
// }


void kernel multires_restrict(global float* U, int res, const int X_SIZE, const int Y_SIZE){
  /* -----------------------------------------------------------------------------
  http://paulbourke.net/miscellaneous/interpolation/
  ----------------------------------------------------------------------------- */
  int g_x = (get_global_id(0)*res)+res;
  int g_y = (get_global_id(1)*res)+res;
  int g_z = (get_global_id(2)*res)+res;

  float value = 0;
  for(int x = 0; x < res; x++){
    for(int y = 0; y < res; y++){
      for(int z = 0; z < res; z++){
        value += U[idx(g_x+x,g_y+y,g_z+z,X_SIZE,Y_SIZE)];
      }
    }
  }
  U[idx(g_x,g_y,g_z,X_SIZE,Y_SIZE)] = value;
}

void kernel multires_interpolate(global float* U, int res, const int X_SIZE, const int Y_SIZE){
  /* -----------------------------------------------------------------------------
  http://paulbourke.net/miscellaneous/interpolation/
  ----------------------------------------------------------------------------- */
  int g_x = (get_global_id(0)*res)+res;
  int g_y = (get_global_id(1)*res)+res;
  int g_z = (get_global_id(2)*res)+res;

  float V000 = (U[idx(g_x,g_y,g_z,X_SIZE,Y_SIZE)]);
  float V001 = (U[idx(g_x,g_y,g_z+res,X_SIZE,Y_SIZE)]);
  float V010 = (U[idx(g_x,g_y+res,g_z,X_SIZE,Y_SIZE)]);
  float V100 = (U[idx(g_x+res,g_y,g_z,X_SIZE,Y_SIZE)]);
  float V101 = (U[idx(g_x+res,g_y,g_z+res,X_SIZE,Y_SIZE)]);
  float V110 = (U[idx(g_x+res,g_y+res,g_z,X_SIZE,Y_SIZE)]);
  float V011 = (U[idx(g_x,g_y+res,g_z+res,X_SIZE,Y_SIZE)]);
  float V111 = (U[idx(g_x+res,g_y+res,g_z+res,X_SIZE,Y_SIZE)]);

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

        U[idx(g_x+x,g_y+y,g_z+z,X_SIZE,Y_SIZE)] = value;
      }
    }
  }

}

void kernel add(global float* input_1, global float* input_2, global float* output){
  output[get_global_id(0)] = input_1[get_global_id(0)]+input_2[get_global_id(0)];
}

void kernel subtract(global float* input_1, global float* input_2, global float* output){
  output[get_global_id(0)] = input_1[get_global_id(0)]-input_2[get_global_id(0)];
}
