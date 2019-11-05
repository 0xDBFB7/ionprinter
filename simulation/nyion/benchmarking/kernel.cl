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

void kernel prolongate_potentials(global float* potentials, global const int* boundaries){
  if(get_global_id(0) > (SIZE*SIZE) && get_global_id(0) < (SIZE*SIZE*SIZE)-((SIZE*SIZE)+1) && !boundaries[get_global_id(0)]){
    potentials[get_global_id(0)] = (potentials[get_global_id(0)+1] +
                                        potentials[get_global_id(0)-1] +
                                        potentials[get_global_id(0)+SIZE] +
                                        potentials[get_global_id(0)-SIZE] +
                                        potentials[get_global_id(0)+(SIZE*SIZE)] +
                                        potentials[get_global_id(0)-(SIZE*SIZE)])/6.0;
  }
}



void kernel weighted_restrict_potentials(global float* potentials, global float* halved_potentials, global const int* boundaries){
  /* -----------------------------------------------------------------------------
  Full-weighting 27-point scheme as described in
  "Three Dimensional Monte Carlo Device Simulation with Parallel Multigrid Solver" -
  "In our experience, a full weighting residual transfer operator is necessary for a stable solution."
  - may not actually be required
  ----------------------------------------------------------------------------- */

  if(get_global_id(0) > (SIZE*SIZE) && get_global_id(0) < (SIZE*SIZE*SIZE)-((SIZE*SIZE)+1) && !boundaries[get_global_id(0)]){
    potentials[get_global_id(0)] =
                            (potentials[get_global_id(0)]*0.125)  +
                            (potentials[get_global_id(0)+1]*0.0625)+
                            (potentials[get_global_id(0)-1]*0.0625)+
                            (potentials[get_global_id(0)+SIZE]*0.0625)+
                            (potentials[get_global_id(0)-SIZE]*0.0625)+
                            (potentials[get_global_id(0)+(SIZE*SIZE)]*0.0625)+
                            (potentials[get_global_id(0)-(SIZE*SIZE)]*0.0625)+

                            (potentials[get_global_id(0)+1+SIZE]*0.03125)+
                            (potentials[get_global_id(0)-1+SIZE]*0.03125)+
                            (potentials[get_global_id(0)+1-SIZE]*0.03125)+
                            (potentials[get_global_id(0)-1-SIZE]*0.03125)+
                            (potentials[get_global_id(0)+1+(SIZE*SIZE)]*0.03125)+
                            (potentials[get_global_id(0)-1+(SIZE*SIZE)]*0.03125)+
                            (potentials[get_global_id(0)+1-(SIZE*SIZE)]*0.03125)+
                            (potentials[get_global_id(0)-1-(SIZE*SIZE)]*0.03125)+
                            (potentials[get_global_id(0)+SIZE+(SIZE*SIZE)]*0.03125)+
                            (potentials[get_global_id(0)-SIZE+(SIZE*SIZE)]*0.03125)+
                            (potentials[get_global_id(0)+SIZE-(SIZE*SIZE)]*0.03125)+
                            (potentials[get_global_id(0)-SIZE-(SIZE*SIZE)]*0.03125);

                            (potentials[get_global_id(0)+1+SIZE+(SIZE*SIZE)]*0.015625)+
                            (potentials[get_global_id(0)-1+SIZE+(SIZE*SIZE)]*0.015625)+
                            (potentials[get_global_id(0)+1-SIZE+(SIZE*SIZE)]*0.015625)+
                            (potentials[get_global_id(0)-1-SIZE+(SIZE*SIZE)]*0.015625)+
                            (potentials[get_global_id(0)+1+SIZE-(SIZE*SIZE)]*0.015625)+
                            (potentials[get_global_id(0)-1+SIZE-(SIZE*SIZE)]*0.015625)+
                            (potentials[get_global_id(0)+1-SIZE-(SIZE*SIZE)]*0.015625)+
                            (potentials[get_global_id(0)-1-SIZE-(SIZE*SIZE)]*0.015625);
  }
}

void kernel jacobi(global const float* potentials, global float* potentials_out, global const int* boundaries){
  if(get_global_id(0) > (SIZE*SIZE) && get_global_id(0) < (SIZE*SIZE*SIZE)-((SIZE*SIZE)+1) && !boundaries[get_global_id(0)]){
    potentials_out[get_global_id(0)] = (potentials[get_global_id(0)+1] +
                                        potentials[get_global_id(0)-1] +
                                        potentials[get_global_id(0)+SIZE] +
                                        potentials[get_global_id(0)-SIZE] +
                                        potentials[get_global_id(0)+(SIZE*SIZE)] +
                                        potentials[get_global_id(0)-(SIZE*SIZE)])/6.0;
  }
}
