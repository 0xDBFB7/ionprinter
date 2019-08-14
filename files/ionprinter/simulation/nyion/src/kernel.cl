#define SIZE 400

void kernel gauss_seidel(global float* potentials, global const int* boundaries){
  if(get_global_id(0) > (SIZE*SIZE) && get_global_id(0) < (SIZE*SIZE*SIZE)-((SIZE*SIZE)+1) && !boundaries[get_global_id(0)]){
    potentials[get_global_id(0)] = (potentials[get_global_id(0)+1] +
                                        potentials[get_global_id(0)-1] +
                                        potentials[get_global_id(0)+SIZE] +
                                        potentials[get_global_id(0)-SIZE] +
                                        potentials[get_global_id(0)+(SIZE*SIZE)] +
                                        potentials[get_global_id(0)-(SIZE*SIZE)])/6.0;
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
//
//
//
// void kernel weighted_restrict(global float* input, global float* output){
//   /* -----------------------------------------------------------------------------
//   Full-weighting 27-point scheme as described in
//   "Three Dimensional Monte Carlo Device Simulation with Parallel Multigrid Solver" -
//   "In our experience, a full weighting residual transfer operator is necessary for a stable solution."
//   ----------------------------------------------------------------------------- */
//     potentials[get_global_id(0)] =
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
