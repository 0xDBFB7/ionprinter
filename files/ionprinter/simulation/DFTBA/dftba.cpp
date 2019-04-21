#include <algorithm>

#define BEAM_COUNT 5

#define SIM_TOTAL_LENGTH 0.01

#define E_MESH_X 50
#define E_MESH_Y 50

// double scharge_efield(float beam_current, float beam_velocity, float beam_radius, float sample_radius){
//     //Calculate the electric field at the edge of a beam
//     //Eq. 44 at https://arxiv.org/pdf/1401.3951.pdf
//     //returns one value: V/m
//     //Beam velocity is in m/s
//
//     if(!sample_radius):
//         sample_radius=beam_radius;
//
//     return ((beam_current/(2.0*(M_PI)*constants.epsilon_0*beam_velocity)) * (sample_radius/(beam_radius**2.0)))
// }

void relax_laplace_potentials(float potentials[][], bool boundary_conditions[][], float tolerance){
  for(int i = 0;i < 1000; i++){
    for(int x = 0; x < E_MESH_X; x++){
      for(int y = 0; y < E_MESH_y; y++){
        if(!boundary_conditions[x][y]){
          
        }
      }
    }
  }
}

int main(){
  float potentials[E_MESH_X][E_MESH_Y] = {0};
  bool boundary_conditions[E_MESH_X][E_MESH_Y] = {0};
  // std::fill(myarray, myarray+, 0);
  // float output_data[BEAM_COUNT][10][] //beam, variable, x slice

}
