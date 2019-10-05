#pragma OPENCL EXTENSION cl_khr_fp16 : enable

#define COEFFICIENT_OF_RELAXATION 0.8

inline int idx(int x, int y, int z, int x_len, int y_len){
  return ((x_len*y_len*z) + (x_len*y) + x);
}

inline float unigrid_directions(int x, int y, int z, int i, int j, int k, int step_size){
  //precompute LUT?
    return ((step_size-abs(x-i))*(step_size-abs(y-j))*(step_size-abs(z-k)))/(step_size*step_size*step_size);
}

void kernel compute_unigrid_corrections(global const half* U, global half* corrections, global const half* F,
                                                            global const half* boundaries,
                                                            int step_size,
                                                            const int SIZE_X, const int SIZE_Y, const int SIZE_Z){
  /* -----------------------------------------------------------------------------
  Compute local corrections
  ----------------------------------------------------------------------------- */

  int x = get_global_id(0)+step_size;
  int y = get_global_id(1)+step_size;
  int z = get_global_id(2)+step_size;

  // int sub_x = x / step_size;
  // sub_x *= step_size; //floor
  // int sub_y = y / step_size;
  // sub_y *= step_size;
  // int sub_z = z / step_size;
  // sub_z *= step_size;
  //
  // int i = x % step_size;
  // int j = y % step_size;
  // int k = z % step_size;

  half residual = 0;
  if(!boundaries[idx(x,y,z,SIZE_X,SIZE_Y)]){
    residual = (6.0f*U[idx(x,y,z,SIZE_X,SIZE_Y)])
                        - U[idx(x+1,y,z,SIZE_X,SIZE_Y)]
                        - U[idx(x-1,y,z,SIZE_X,SIZE_Y)]
                        - U[idx(x,y+1,z,SIZE_X,SIZE_Y)]
                        - U[idx(x,y-1,z,SIZE_X,SIZE_Y)]
                        - U[idx(x,y,z+1,SIZE_X,SIZE_Y)]
                        - U[idx(x,y,z-1,SIZE_X,SIZE_Y)]
                        - F[idx(x,y,z,SIZE_X,SIZE_Y)];
                        //unigrid works backwards from residual.
  }

  // A1 and A3 can be pre-computed.
  // half A3 = 6.0f*unigrid_directions(x,y,z,i,j,k,step_size)
  //                   - unigrid_directions(x,y,z,i-1,j,k,step_size)
  //                   - unigrid_directions(x,y,z,i+1,j,k,step_size)
  //                   - unigrid_directions(x,y,z,i,j-1,k,step_size)
  //                   - unigrid_directions(x,y,z,i,j+1,k,step_size)
  //                   - unigrid_directions(x,y,z,i,j,k-1,step_size)
  //                   - unigrid_directions(x,y,z,i,j,k+1,step_size);
  //  = unigrid_directions(x,y,z,i,j,k,step_size) * A3;

  corrections[idx(x,y,z,SIZE_X,SIZE_Y)] = residual;
}



//
// void kernel apply_unigrid_corrections(global const float* U, global float* output, global const float* F,
//                                                             global const float* boundaries,
//                                                             int step_size,
//                                                             const int SIZE_X, const int SIZE_Y, const int SIZE_Z){
//   /* -----------------------------------------------------------------------------
//
//   ----------------------------------------------------------------------------- */
//   int x = (get_global_id(0)*step_size)+step_size;
//   int y = (get_global_id(1)*step_size)+step_size;
//   int z = (get_global_id(2)*step_size)+step_size;
//
//
//   /* -----------------------------------------------------------------------------
//   Fix coarse boundaries if required
//   ----------------------------------------------------------------------------- */
//   // for(int i = x-step_size+1; i < x+step_size; i++){
//   //   for(int j = y-step_size+1; j < y+step_size; j++){
//   //     for(int k = z-step_size+1; k < z+step_size; k++){
//   //
//   //     }
//   //   }
//   // }
//   // if(B[I-M1:I+M1,J-M1:J+M1].max() and B[I-M1:I+M1,J-M1:J+M1].sum() != 9 and k < 2){
//   //  for(int i = x-step_size+1; i < x+step_size; i++){
//     //   for(int j = y-step_size+1; j < y+step_size; j++){
//     //     for(int k = z-step_size+1; i < z+step_size; z++){
//     //
//     //     }
//     //   }
//     // }
//   // }
//
//
//   half correction = R1/A1;
//   // /* -----------------------------------------------------------------------------
//   // Apply correction to local region
//   // ----------------------------------------------------------------------------- */
//   for(int i = x-step_size+1; i < x+step_size; i++){
//     for(int j = y-step_size+1; j < y+step_size; j++){
//       for(int k = z-step_size+1; k < z+step_size; k++){
//         output[idx(i,j,k,SIZE_X,SIZE_Y)] = 2.0;
//         // if(!boundaries[idx(i,j,k,SIZE_X,SIZE_Y)]){
//         //   output[idx(i,j,k,SIZE_X,SIZE_Y)] = U[idx(i,j,k,SIZE_X,SIZE_Y)] -
//         //                 (COEFFICIENT_OF_RELAXATION * correction * unigrid_directions(x,y,z,i,j,k,step_size));
//         // }
//         // else{
//         //   output[idx(i,j,k,SIZE_X,SIZE_Y)] = U[idx(i,j,k,SIZE_X,SIZE_Y)];
//         // }
//       }
//     }
//   }
// }

void kernel multires_jacobi(global const half* U, global half* T, global const half* RHS, global const half* boundaries, int res, const int X_SIZE, const int Y_SIZE){
    /* -----------------------------------------------------------------------------
    Call with a 3d NDRange of DIM_SIZE - 2.
    The +1 offset is added to exclude the invalid 0 borders of the mesh; the - 2 handles the other edge.

    ----------------------------------------------------------------------------- */
    int x = (get_global_id(0)*res)+res;
    int y = (get_global_id(1)*res)+res;
    int z = (get_global_id(2)*res)+res;
  //
    int coord = idx(x,y,z,X_SIZE,Y_SIZE);
    if(!boundaries[idx(x,y,z,X_SIZE,Y_SIZE)]){
      T[coord] = ((6.0f*U[idx(x,y,z,X_SIZE,Y_SIZE)]) -
                  U[idx(x+res,y,z,X_SIZE,Y_SIZE)] -
                  U[idx(x-res,y,z,X_SIZE,Y_SIZE)] -
                  U[idx(x,y+res,z,X_SIZE,Y_SIZE)] -
                  U[idx(x,y-res,z,X_SIZE,Y_SIZE)] -
                  U[idx(x,y,z+res,X_SIZE,Y_SIZE)] -
                  U[idx(x,y,z-res,X_SIZE,Y_SIZE)] -
                  RHS[idx(x,y,z,X_SIZE,Y_SIZE)]);
    }
}

void kernel add(global half* input_1, global half* input_2, global half* output){
  output[get_global_id(0)] = input_1[get_global_id(0)]+input_2[get_global_id(0)];
}

void kernel subtract(global half* input_1, global half* input_2, global half* output){
  output[get_global_id(0)] = input_1[get_global_id(0)]-input_2[get_global_id(0)];
}
