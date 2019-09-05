
#define COEFFICIENT_OF_RELAXATION 0.8

inline int idx(int x, int y, int z, int x_len, int y_len){
  return ((x_len*y_len*z) + (x_len*y) + x);
}

inline float unigrid_directions(int x, int y, int z, int i, int j, int k, int step_size){
  //precompute LUT?
    return ((step_size-abs(x-i))*(step_size-abs(y-j))*(step_size-abs(z-k)))/(step_size*step_size*step_size);
}

void kernel unigrid(global const float* U, global float* output, global const float* F,
                                                            global const float* boundaries,
                                                            int step_size,
                                                            const int SIZE_X, const int SIZE_Y, const int SIZE_Z){
  /* -----------------------------------------------------------------------------

  ----------------------------------------------------------------------------- */
  int x = (get_global_id(0)*step_size)+step_size;
  int y = (get_global_id(1)*step_size)+step_size;
  int z = (get_global_id(2)*step_size)+step_size;


  /* -----------------------------------------------------------------------------
  Fix coarse boundaries if required
  ----------------------------------------------------------------------------- */
  // if(B[I-M1:I+M1,J-M1:J+M1].max() and B[I-M1:I+M1,J-M1:J+M1].sum() != 9 and k < 2){
  //  for(int i = x-step_size+1; i < x+step_size; i++){
    //   for(int j = y-step_size+1; j < y+step_size; j++){
    //     for(int k = z-step_size+1; i < z+step_size; z++){
    //
    //     }
    //   }
    // }
  // }

  /* -----------------------------------------------------------------------------
  Compute local correction
  ----------------------------------------------------------------------------- */
  float A1 = 0;
  float R1 = 0;
  for(int i = x-step_size+1; i < x+step_size; i++){
    for(int j = y-step_size+1; j < y+step_size; j++){
      for(int k = z-step_size+1; k < z+step_size; k++){
        float residual = (6.0*U[idx(i,j,k,SIZE_X,SIZE_Y)])
                            - U[idx(i+1,j,k,SIZE_X,SIZE_Y)]
                            - U[idx(i-1,j,k,SIZE_X,SIZE_Y)]
                            - U[idx(i,j+1,k,SIZE_X,SIZE_Y)]
                            - U[idx(i,j-1,k,SIZE_X,SIZE_Y)]
                            - U[idx(i,j,k+1,SIZE_X,SIZE_Y)]
                            - U[idx(i,j,k-1,SIZE_X,SIZE_Y)]
                            - F[idx(i,j,k,SIZE_X,SIZE_Y)];
                            //unigrid works backwards from residual.


        if(!boundaries[idx(i,j,k,SIZE_X,SIZE_Y)]){
          R1 += (unigrid_directions(x,y,z,i,j,k,step_size)*residual);
        }
        float A3 = 6.0*unigrid_directions(x,y,z,i,j,k,step_size)
                          - unigrid_directions(x+1,y,z,i,j,k,step_size)
                          - unigrid_directions(x-1,y,z,i,j,k,step_size)
                          - unigrid_directions(x,y+1,z,i,j,k,step_size)
                          - unigrid_directions(x,y-1,z,i,j,k,step_size)
                          - unigrid_directions(x,y,z+1,i,j,k,step_size)
                          - unigrid_directions(x,y,z-1,i,j,k,step_size);
        A1 += unigrid_directions(x,y,z,i,j,k,step_size) * A3;
      }
    }
  }
  float correction = R1/A1;
  // /* -----------------------------------------------------------------------------
  // Apply correction to local region
  // ----------------------------------------------------------------------------- */
  for(int i = x-step_size+1; i < x+step_size; i++){
    for(int j = y-step_size+1; j < y+step_size; j++){
      for(int k = z-step_size+1; k < z+step_size; k++){
        if(!boundaries[idx(i,j,k,SIZE_X,SIZE_Y)]){
          output[idx(i,j,k,SIZE_X,SIZE_Y)] = U[idx(i,j,k,SIZE_X,SIZE_Y)] -
                        (COEFFICIENT_OF_RELAXATION * correction * unigrid_directions(x,y,z,i,j,k,step_size));
        }
      }
    }
  }
}

// void kernel multires_gauss_seidel(global float* U, global const float* b, int res, const int X_SIZE, const int Y_SIZE){
//     /* -----------------------------------------------------------------------------
//     Call with a 3d NDRange of DIM_SIZE - 2.
//     The +1 offset is added to exclude the invalid 0 borders of the mesh; the - 2 handles the other edge.
//
//     ----------------------------------------------------------------------------- */
//     // int x = (get_global_id(0)*res)+res;
//     // int y = (get_global_id(1)*res)+res;
//     // int z = (get_global_id(2)*res)+res;
//
//     for(int x = res; x < (X_SIZE-res)-1; x += res){
//       for(int y = res; y < (X_SIZE-res)-1; y += res){
//         for(int z = res; z < (X_SIZE-res)-1; z += res){
//           int coord = idx(x,y,z,X_SIZE,Y_SIZE);
//           U[coord] = (U[idx(x+res,y,z,X_SIZE,Y_SIZE)] +
//                       U[idx(x-res,y,z,X_SIZE,Y_SIZE)] +
//                       U[idx(x,y+res,z,X_SIZE,Y_SIZE)] +
//                       U[idx(x,y-res,z,X_SIZE,Y_SIZE)] +
//                       U[idx(x,y,z+res,X_SIZE,Y_SIZE)] +
//                       U[idx(x,y,z-res,X_SIZE,Y_SIZE)] +
//                       b[idx(x,y,z,X_SIZE,Y_SIZE)])/6.0;
//         }
//       }
//     }
// }
//
// void kernel multires_jacobi(global float* U, global float* T, global const float* b, int res, const int X_SIZE, const int Y_SIZE){
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
//     T[coord] = (U[idx(x+res,y,z,X_SIZE,Y_SIZE)] +
//                 U[idx(x-res,y,z,X_SIZE,Y_SIZE)] +
//                 U[idx(x,y+res,z,X_SIZE,Y_SIZE)] +
//                 U[idx(x,y-res,z,X_SIZE,Y_SIZE)] +
//                 U[idx(x,y,z+res,X_SIZE,Y_SIZE)] +
//                 U[idx(x,y,z-res,X_SIZE,Y_SIZE)] +
//                 b[idx(x,y,z,X_SIZE,Y_SIZE)])/6.0;
// }
//
//
// void kernel multires_restrict(global float* U, int res, const int X_SIZE, const int Y_SIZE){
//   /* -----------------------------------------------------------------------------
//   http://paulbourke.net/miscellaneous/interpolation/
//   ----------------------------------------------------------------------------- */
//   int g_x = (get_global_id(0)*res);
//   int g_y = (get_global_id(1)*res);
//   int g_z = (get_global_id(2)*res);
//
//   float value = 0;
//   for(int x = 0; x < res; x++){
//     for(int y = 0; y < res; y++){
//       for(int z = 0; z < res; z++){
//         value += U[idx(g_x+x,g_y+y,g_z+z,X_SIZE,Y_SIZE)];
//       }
//     }
//   }
//   U[idx(g_x,g_y,g_z,X_SIZE,Y_SIZE)] = value;
// }
//
// void kernel multires_interpolate(global float* U, int res, const int X_SIZE, const int Y_SIZE){
//   /* -----------------------------------------------------------------------------
//   http://paulbourke.net/miscellaneous/interpolation/
//   ----------------------------------------------------------------------------- */
//   int g_x = (get_global_id(0)*res);
//   int g_y = (get_global_id(1)*res);
//   int g_z = (get_global_id(2)*res);
//
//   float V000 = (U[idx(g_x,g_y,g_z,X_SIZE,Y_SIZE)]);
//   float V001 = (U[idx(g_x,g_y,g_z+res,X_SIZE,Y_SIZE)]);
//   float V010 = (U[idx(g_x,g_y+res,g_z,X_SIZE,Y_SIZE)]);
//   float V100 = (U[idx(g_x+res,g_y,g_z,X_SIZE,Y_SIZE)]);
//   float V101 = (U[idx(g_x+res,g_y,g_z+res,X_SIZE,Y_SIZE)]);
//   float V110 = (U[idx(g_x+res,g_y+res,g_z,X_SIZE,Y_SIZE)]);
//   float V011 = (U[idx(g_x,g_y+res,g_z+res,X_SIZE,Y_SIZE)]);
//   float V111 = (U[idx(g_x+res,g_y+res,g_z+res,X_SIZE,Y_SIZE)]);
//
//   for(int x = 0; x < res; x++){
//     for(int y = 0; y < res; y++){
//       for(int z = 0; z < res; z++){
//         float f_x = (((float)x)/res);
//         float f_y = (((float)y)/res);
//         float f_z = (((float)z)/res);
//
//         float inverse_x = 1.0-f_x;
//         float inverse_y = 1.0-f_y;
//         float inverse_z = 1.0-f_z;
//
//         float value = 0;
//
//         value += V000*(inverse_x)*(inverse_y)*(inverse_z);
//         value += V100*(f_x)*(inverse_y)*(inverse_z);
//         value += V010*(inverse_x)*(f_y)*(inverse_z);
//         value += V001*(inverse_x)*(inverse_y)*(f_z);
//         value += V101*(f_x)*(inverse_y)*(f_z);
//         value += V011*(inverse_x)*(f_y)*(f_z);
//         value += V110*(f_x)*(f_y)*(inverse_z);
//         value += V111*(f_x)*(f_y)*(f_z);
//
//         U[idx(g_x+x,g_y+y,g_z+z,X_SIZE,Y_SIZE)] = value;
//       }
//     }
//   }
//
// }

void kernel add(global float* input_1, global float* input_2, global float* output){
  output[get_global_id(0)] = input_1[get_global_id(0)]+input_2[get_global_id(0)];
}

void kernel subtract(global float* input_1, global float* input_2, global float* output){
  output[get_global_id(0)] = input_1[get_global_id(0)]-input_2[get_global_id(0)];
}
