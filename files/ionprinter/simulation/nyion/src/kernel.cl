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

void kernel weighted_restrict(global float* input, global float* output){
  /* -----------------------------------------------------------------------------
  Full-weighting 27-point scheme as described in
  "Three Dimensional Monte Carlo Device Simulation with Parallel Multigrid Solver" -
  "In our experience, a full weighting residual transfer operator is necessary for a stable solution."
  Large input -> small output.
  ----------------------------------------------------------------------------- */
  int IN_X_SIZE = ((get_global_size(0)+2)*2);
  int IN_Y_SIZE = ((get_global_size(1)+2)*2);
  int OUT_X_SIZE = get_global_size(0)+2;
  int OUT_Y_SIZE = get_global_size(1)+2;

  int o_x = get_global_id(0)+1; //subtract first row
  int o_y = get_global_id(1)+1;
  int o_z = get_global_id(2)+1;

  int i_x = ((get_global_id(0)+1)*2);
  int i_y = ((get_global_id(1)+1)*2);
  int i_z = ((get_global_id(2)+1)*2);

  output[idx(o_x,o_y,o_z,OUT_X_SIZE,OUT_Y_SIZE)] =
                            (input[idx(i_x,i_y,i_z,IN_X_SIZE,IN_Y_SIZE)]*0.125)   +

                            ((input[idx(i_x+1,i_y,i_z,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x-1,i_y,i_z,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x,i_y+1,i_z,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x,i_y-1,i_z,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x,i_y,i_z+1,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x,i_y,i_z-1,IN_X_SIZE,IN_Y_SIZE)])*0.5)+

                            ((input[idx(i_x+1,i_y+1,i_z,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x-1,i_y+1,i_z,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x+1,i_y-1,i_z,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x-1,i_y-1,i_z,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x+1,i_y,i_z+1,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x-1,i_y,i_z+1,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x+1,i_y,i_z-1,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x-1,i_y,i_z-1,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x,i_y+1,i_z+1,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x,i_y-1,i_z+1,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x,i_y+1,i_z-1,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x,i_y-1,i_z-1,IN_X_SIZE,IN_Y_SIZE)])*0.375)+

                            ((input[idx(i_x+1,i_y+1,i_z+1,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x-1,i_y+1,i_z+1,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x+1,i_y-1,i_z+1,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x-1,i_y-1,i_z+1,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x+1,i_y+1,i_z-1,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x-1,i_y+1,i_z-1,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x+1,i_y-1,i_z-1,IN_X_SIZE,IN_Y_SIZE)]+
                            input[idx(i_x-1,i_y-1,i_z-1,IN_X_SIZE,IN_Y_SIZE)])*0.125);
}

void kernel interpolate(global float* input, global float* output){
  /* -----------------------------------------------------------------------------
  NDRange must be between 0 and input-1.
  See Chapter 8, Page 35, "A multigrid tutorial", Briggs et al.
  Small input -> large output.
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

void kernel add(global float* input_1, global float* input_2, global float* output){
  output[get_global_id(0)] = input_1[get_global_id(0)]+input_2[get_global_id(0)];
}


void kernel subtract(global float* input_1, global float* input_2, global float* output){
  output[get_global_id(0)] = input_1[get_global_id(0)]-input_2[get_global_id(0)];
}

void kernel simple_injection(global float* input, global float* output){
  int o_idx = idx((get_global_id(0)+1),
                  (get_global_id(1)+1),
                  (get_global_id(2)+1),get_global_size(0)+2,get_global_size(1)+2);
  output[o_idx] = input[idx(2*(get_global_id(0)+1),
                                        2*(get_global_id(1)+1),
                                        2*(get_global_id(2)+1),get_global_size(0)+2,get_global_size(1)+2)];
}

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