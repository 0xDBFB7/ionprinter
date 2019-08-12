#define SIZE 450


void kernel simple_add(global const double* potentials, global double* potentials_out, global const int* boundaries){
  if(get_global_id(0) > (SIZE*SIZE) && get_global_id(0) < (SIZE*SIZE*SIZE)-((SIZE*SIZE)+1) && !boundaries[get_global_id(0)]){
    potentials_out[get_global_id(0)] = (potentials[get_global_id(0)+1] +
                                        potentials[get_global_id(0)-1] +
                                        potentials[get_global_id(0)+SIZE] +
                                        potentials[get_global_id(0)-SIZE] +
                                        potentials[get_global_id(0)+(SIZE*SIZE)] +
                                        potentials[get_global_id(0)-(SIZE*SIZE)])/6.0;
  }
}
