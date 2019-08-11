void kernel simple_add(global const double* A, global const double* B, global double* C){
                 C[get_global_id(0)]=A[get_global_id(0)]+B[get_global_id(0)];
           }
