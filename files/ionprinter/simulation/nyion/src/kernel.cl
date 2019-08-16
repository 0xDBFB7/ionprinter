#define SIZE_X 250
#define SIZE_Y 250
#define SIZE_Z 250

#define SIZE_XY (SIZE_X*SIZE_Y)
#define SIZE_XYZ (SIZE_X*SIZE_Y*SIZE_Z)

inline int idx(int x, int y, int z, int x_len, int y_len){
  return ((x_len*y_len*z) + (x_len*y) + x);
}

void kernel gauss_seidel(global float* potentials, global const int* boundaries){
    /* -----------------------------------------------------------------------------
    Call with a 3d NDRange of DIM_SIZE - 2.
    The +1 offset is added to exclude the invalid 0 borders of the mesh; the - 2 handles the other edge.
    ----------------------------------------------------------------------------- */
    int coord = idx(get_global_id(0)+1,get_global_id(1)+1,get_global_id(2)+1,(get_global_size(0)+2),(get_global_size(1)+2));
    if(!boundaries[coord]){
      potentials[coord] =     (potentials[coord+1] +
                                    potentials[coord-1] +
                                    potentials[coord+(get_global_size(0)+2)] +
                                    potentials[coord-(get_global_size(0)+2)] +
                                    potentials[coord+((get_global_size(0)+2)*(get_global_size(1)+2))] +
                                    potentials[coord-((get_global_size(0)+2)*(get_global_size(1)+2))])/6.0;
    }
}

//
// void kernel prolongate_potentials(global float* potentials, global const int* boundaries){
//   if(get_global_id(0) > (SIZE*SIZE) && get_global_id(0) < (SIZE*SIZE*SIZE)-((SIZE*SIZE)+1) && !boundaries[get_global_id(0)]){
//     potentials[get_global_id(0)] = (potentials[get_global_id(0)+1] +
//                                         potentials[get_global_id(0)-1] +
//                                         potentials[get_global_id(0)+SIZE] +
//                                         potentials[get_global_id(0)-SIZE] +
//                                         potentials[get_global_id(0)+(SIZE*SIZE)] +
//                                         potentials[get_global_id(0)-(SIZE*SIZE)])/6.0;
//   }
// }


void kernel weighted_restrict(global float* input, global float* output){
  /* -----------------------------------------------------------------------------
  Full-weighting 27-point scheme as described in
  "Three Dimensional Monte Carlo Device Simulation with Parallel Multigrid Solver" -
  "In our experience, a full weighting residual transfer operator is necessary for a stable solution."
  ----------------------------------------------------------------------------- */
  int input_coord = idx(get_global_id(0)+1,get_global_id(1)+1,get_global_id(2)+1,(get_global_size(0)+2),(get_global_size(1)+2));
  int output_coord = idx((get_global_id(0)/2)+1,(get_global_id(1)/2)+1,(get_global_id(2)/2)+1,(get_global_size(0)/2)+1,get_global_size(1));
    output[output_coord] =
                            (input[get_global_id(0)]*0.125)  +
                            (input[get_global_id(0)+1]*0.0625)+
                            (input[get_global_id(0)-1]*0.0625)+
                            (input[get_global_id(0)+SIZE_X]*0.0625)+
                            (input[get_global_id(0)-SIZE_X]*0.0625)+
                            (input[get_global_id(0)+(SIZE_X*SIZE_Y)]*0.0625)+
                            (input[get_global_id(0)-(SIZE_X*SIZE_Y)]*0.0625)+

                            (input[get_global_id(0)+1+SIZE_X]*0.03125)+
                            (input[get_global_id(0)-1+SIZE_X]*0.03125)+
                            (input[get_global_id(0)+1-SIZE_X]*0.03125)+
                            (input[get_global_id(0)-1-SIZE_X]*0.03125)+
                            (input[get_global_id(0)+1+(SIZE_X*SIZE_Y)]*0.03125)+
                            (input[get_global_id(0)-1+(SIZE_X*SIZE_Y)]*0.03125)+
                            (input[get_global_id(0)+1-(SIZE_X*SIZE_Y)]*0.03125)+
                            (input[get_global_id(0)-1-(SIZE_X*SIZE_Y)]*0.03125)+
                            (input[get_global_id(0)+SIZE_X+(SIZE_X*SIZE_Y)]*0.03125)+
                            (input[get_global_id(0)-SIZE_X+(SIZE_X*SIZE_Y)]*0.03125)+
                            (input[get_global_id(0)+SIZE_X-(SIZE_X*SIZE_Y)]*0.03125)+
                            (input[get_global_id(0)-SIZE_X-(SIZE_X*SIZE_Y)]*0.03125)+

                            (input[get_global_id(0)+1+SIZE_X+(SIZE_X*SIZE_Y)]*0.015625)+
                            (input[get_global_id(0)-1+SIZE_X+(SIZE_X*SIZE_Y)]*0.015625)+
                            (input[get_global_id(0)+1-SIZE_X+(SIZE_X*SIZE_Y)]*0.015625)+
                            (input[get_global_id(0)-1-SIZE_X+(SIZE_X*SIZE_Y)]*0.015625)+
                            (input[get_global_id(0)+1+SIZE_X-(SIZE_X*SIZE_Y)]*0.015625)+
                            (input[get_global_id(0)-1+SIZE_X-(SIZE_X*SIZE_Y)]*0.015625)+
                            (input[get_global_id(0)+1-SIZE_X-(SIZE_X*SIZE_Y)]*0.015625)+
                            (input[get_global_id(0)-1-SIZE_X-(SIZE_X*SIZE_Y)]*0.015625);
}

void kernel interpolate(global float* input, global float* output){
  /* -----------------------------------------------------------------------------
  NDRange must be between 0 and input-1.
  See Chapter 8, Page 35, "A multigrid tutorial", Briggs et al.
  ----------------------------------------------------------------------------- */
  int OUT_X_SIZE = ((get_global_size(0)+1)*2);
  int OUT_Y_SIZE = ((get_global_size(1)+1)*2);
  int IN_X_SIZE = get_global_size(0)+1;
  int IN_Y_SIZE = get_global_size(1)+1;

  output[idx(get_global_id(0)*2,get_global_id(1)*2,get_global_id(2)*2,OUT_X_SIZE,OUT_Y_SIZE)] =
        input[idx(get_global_id(0),get_global_id(1),get_global_id(2),IN_X_SIZE,IN_Y_SIZE)];

  output[idx((get_global_id(0)*2)+1,(get_global_id(1)*2),(get_global_id(2)*2),OUT_X_SIZE,OUT_Y_SIZE)] =
        (input[idx(get_global_id(0),get_global_id(1),get_global_id(2),IN_X_SIZE,IN_Y_SIZE)] +
        input[idx(get_global_id(0)+1,get_global_id(1),get_global_id(2),IN_X_SIZE,IN_Y_SIZE)])/2.0;

  output[idx((get_global_id(0)*2),(get_global_id(1)*2)+1,(get_global_id(2)*2),OUT_X_SIZE,OUT_Y_SIZE)] =
        (input[idx(get_global_id(0),get_global_id(1),get_global_id(2),IN_X_SIZE,IN_Y_SIZE)] +
        input[idx(get_global_id(0),get_global_id(1)+1,get_global_id(2),IN_X_SIZE,IN_Y_SIZE)])/2.0;

  output[idx((get_global_id(0)*2),(get_global_id(1)*2),(get_global_id(2)*2)+1,OUT_X_SIZE,OUT_Y_SIZE)] =
        (input[idx(get_global_id(0),get_global_id(1),get_global_id(2),IN_X_SIZE,IN_Y_SIZE)] +
        input[idx(get_global_id(0),get_global_id(1),get_global_id(2)+1,IN_X_SIZE,IN_Y_SIZE)])/2.0;

  output[idx((get_global_id(0)*2)+1,(get_global_id(1)*2)+1,(get_global_id(2)*2),OUT_X_SIZE,OUT_Y_SIZE)] =
       (input[idx(get_global_id(0),get_global_id(1),get_global_id(2),IN_X_SIZE,IN_Y_SIZE)] +
        input[idx(get_global_id(0)+1,get_global_id(1),get_global_id(2),IN_X_SIZE,IN_Y_SIZE)] +
        input[idx(get_global_id(0),get_global_id(1)+1,get_global_id(2),IN_X_SIZE,IN_Y_SIZE)] +
        input[idx(get_global_id(0)+1,get_global_id(1)+1,get_global_id(2),IN_X_SIZE,IN_Y_SIZE)])/4.0;

  output[idx((get_global_id(0)*2),(get_global_id(1)*2)+1,(get_global_id(2)*2)+1,OUT_X_SIZE,OUT_Y_SIZE)] =
       (input[idx(get_global_id(0),get_global_id(1),get_global_id(2),IN_X_SIZE,IN_Y_SIZE)] +
        input[idx(get_global_id(0),get_global_id(1)+1,get_global_id(2),IN_X_SIZE,IN_Y_SIZE)] +
        input[idx(get_global_id(0),get_global_id(1),get_global_id(2)+1,IN_X_SIZE,IN_Y_SIZE)] +
        input[idx(get_global_id(0),get_global_id(1)+1,get_global_id(2)+1,IN_X_SIZE,IN_Y_SIZE)])/4.0;

  output[idx((get_global_id(0)*2)+1,(get_global_id(1)*2),(get_global_id(2)*2)+1,OUT_X_SIZE,OUT_Y_SIZE)] =
       (input[idx(get_global_id(0),get_global_id(1),get_global_id(2),IN_X_SIZE,IN_Y_SIZE)] +
        input[idx(get_global_id(0)+1,get_global_id(1),get_global_id(2),IN_X_SIZE,IN_Y_SIZE)] +
        input[idx(get_global_id(0),get_global_id(1),get_global_id(2)+1,IN_X_SIZE,IN_Y_SIZE)] +
        input[idx(get_global_id(0)+1,get_global_id(1),get_global_id(2)+1,IN_X_SIZE,IN_Y_SIZE)])/4.0;

  output[idx((get_global_id(0)*2)+1,(get_global_id(1)*2)+1,(get_global_id(2)*2)+1,OUT_X_SIZE,OUT_Y_SIZE)] =
       (input[idx(get_global_id(0),get_global_id(1),get_global_id(2),IN_X_SIZE,IN_Y_SIZE)] +
        input[idx(get_global_id(0)+1,get_global_id(1),get_global_id(2),IN_X_SIZE,IN_Y_SIZE)] +
        input[idx(get_global_id(0),get_global_id(1)+1,get_global_id(2),IN_X_SIZE,IN_Y_SIZE)] +
        input[idx(get_global_id(0),get_global_id(1),get_global_id(2)+1,IN_X_SIZE,IN_Y_SIZE)] +
        input[idx(get_global_id(0)+1,get_global_id(1)+1,get_global_id(2),IN_X_SIZE,IN_Y_SIZE)] +
        input[idx(get_global_id(0),get_global_id(1)+1,get_global_id(2)+1,IN_X_SIZE,IN_Y_SIZE)] +
        input[idx(get_global_id(0)+1,get_global_id(1),get_global_id(2)+1,IN_X_SIZE,IN_Y_SIZE)] +
        input[idx(get_global_id(0)+1,get_global_id(1)+1,get_global_id(2)+1,IN_X_SIZE,IN_Y_SIZE)])/8.0;
}


//
// void kernel simple_restrict(global float* input, global float* output){
//   /* -----------------------------------------------------------------------------
//   Full-weighting 27-point scheme as described in
//   "Three Dimensional Monte Carlo Device Simulation with Parallel Multigrid Solver" -
//   "In our experience, a full weighting residual transfer operator is necessary for a stable solution."
//   ----------------------------------------------------------------------------- */
//   int input_coord = idx(get_global_id(0)+1,get_global_id(1)+1,get_global_id(2)+1,(get_global_size(0)+2),(get_global_size(1)+2));
//   int output_coord = idx((get_global_id(0)/2)+1,get_global_id(1)/2)+1,get_global_id(2)/2)+1,(get_global_size(0)/2)+1,get_global_size(1));
//     output[output_coord] =
//                             (input[get_global_id(0)]*0.125)  +
//                             (input[get_global_id(0)+1]*0.0625)+
//                             (input[get_global_id(0)-1]*0.0625)+
//                             (input[get_global_id(0)+SIZE_X]*0.0625)+
//                             (input[get_global_id(0)-SIZE_X]*0.0625)+
//                             (input[get_global_id(0)+(SIZE_X*SIZE_Y)]*0.0625)+
//                             (input[get_global_id(0)-(SIZE_X*SIZE_Y)]*0.0625)+
//
//                             (input[get_global_id(0)+1+SIZE_X]*0.03125)+
//                             (input[get_global_id(0)-1+SIZE_X]*0.03125)+
//                             (input[get_global_id(0)+1-SIZE_X]*0.03125)+
//                             (input[get_global_id(0)-1-SIZE_X]*0.03125)+
//                             (input[get_global_id(0)+1+(SIZE_X*SIZE_Y)]*0.03125)+
//                             (input[get_global_id(0)-1+(SIZE_X*SIZE_Y)]*0.03125)+
//                             (input[get_global_id(0)+1-(SIZE_X*SIZE_Y)]*0.03125)+
//                             (input[get_global_id(0)-1-(SIZE_X*SIZE_Y)]*0.03125)+
//                             (input[get_global_id(0)+SIZE_X+(SIZE_X*SIZE_Y)]*0.03125)+
//                             (input[get_global_id(0)-SIZE_X+(SIZE_X*SIZE_Y)]*0.03125)+
//                             (input[get_global_id(0)+SIZE_X-(SIZE_X*SIZE_Y)]*0.03125)+
//                             (input[get_global_id(0)-SIZE_X-(SIZE_X*SIZE_Y)]*0.03125);
//
//                             (input[get_global_id(0)+1+SIZE_X+(SIZE_X*SIZE_Y)]*0.015625)+
//                             (input[get_global_id(0)-1+SIZE_X+(SIZE_X*SIZE_Y)]*0.015625)+
//                             (input[get_global_id(0)+1-SIZE_X+(SIZE_X*SIZE_Y)]*0.015625)+
//                             (input[get_global_id(0)-1-SIZE_X+(SIZE_X*SIZE_Y)]*0.015625)+
//                             (input[get_global_id(0)+1+SIZE_X-(SIZE_X*SIZE_Y)]*0.015625)+
//                             (input[get_global_id(0)-1+SIZE_X-(SIZE_X*SIZE_Y)]*0.015625)+
//                             (input[get_global_id(0)+1-SIZE_X-(SIZE_X*SIZE_Y)]*0.015625)+
//                             (input[get_global_id(0)-1-SIZE_X-(SIZE_X*SIZE_Y)]*0.015625);
// }

//
// void kernel jacobi(global const float* potentials, global float* potentials_out, global const int* boundaries){
//   if(get_global_id(0) > (SIZE*SIZE) && get_global_id(0) < (SIZE*SIZE*SIZE)-((SIZE*SIZE)+1) && !boundaries[get_global_id(0)]){
//     potentials_out[get_global_id(0)] = (potentials[get_global_id(0)+1] +
//                                         potentials[get_global_id(0)-1] +
//                                         potentials[get_global_id(0)+SIZE] +
//                                         potentials[get_global_id(0)-SIZE] +
//                                         potentials[get_global_id(0)+(SIZE*SIZE)] +
//                                         potentials[get_global_id(0)-(SIZE*SIZE)])/6.0;
//   }
// }
