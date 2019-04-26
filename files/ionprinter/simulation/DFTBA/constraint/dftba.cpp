#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <chrono>
using namespace std::chrono;

#define BEAM_COUNT 5



#define E_MESH_X 50
#define E_MESH_Y 50

#define MESH_SCALE_X 0.0001 //0.00
#define MESH_SCALE_Y 0.0001


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

float float_array_max(float array[E_MESH_X][E_MESH_Y]){
  float max = 0;
  for(int x = 0; x < E_MESH_X; x++){
    for(int y = 0; y < E_MESH_Y; y++){
      if(array[x][y] > max){
        max = array[x][y];
      }
    }
  }
  return max;
}

float float_array_min(float array[E_MESH_X][E_MESH_Y]){
  float min = 0;
  for(int x = 0; x < E_MESH_X; x++){
    for(int y = 0; y < E_MESH_Y; y++){
        if(array[x][y] < min){
          min = array[x][y];
      }
    }
  }
  return min;
}

void relax_laplace_potentials(float potentials[E_MESH_X][E_MESH_Y], bool boundary_conditions[E_MESH_X][E_MESH_Y], float tolerance){
  // int convergence_index_x = 0;//Find a point to sample for convergence
  // int convergence_index_y = 0;

  // for(int x = 1; x < E_MESH_X-1; x++){
  //   for(int y = 1; y < E_MESH_Y-1; y++){
  //     if(boundary_conditions[x][y]){

  for(int i = 0; i < 3000; i++){
    for(int x = 1; x < E_MESH_X-1; x++){
      for(int y = 1; y < E_MESH_Y-1; y++){
        if(!boundary_conditions[x][y]){
          potentials[x][y] = (potentials[x-1][y]+potentials[x+1][y]+potentials[x][y+1]+potentials[x][y-1])/4.0;
          // printf("%f\n",potentials[x][y]);
        }
      }
    }
    // new_convergence = std::abs(potentials[x][y]);
  }
}

float x_electric_field_at_position(float potentials[E_MESH_X][E_MESH_Y],int x,int y){
  return (((potentials[x][y]-potentials[x-1][y])+(potentials[x+1][y]-potentials[x][y]))/2.0)/MESH_SCALE_X;
  // # sub_array = potentials[int(particle_position[Y]*mesh_scale_y):int(particle_position[Y]*mesh_scale_y)+2,
  // #                         int(particle_position[X]*mesh_scale_x):int(particle_position[X]*mesh_scale_x)+2]
  // #
  // # grad = np.gradient(sub_array)[0]
  // # electric_field_x = grad[0][0]*mesh_scale_x #convert to volts/meter
  // # electric_field_y = grad[1][0]*mesh_scale_y
}

float y_electric_field_at_position(float potentials[E_MESH_X][E_MESH_Y],int x,int y){
  return (((potentials[x][y]-potentials[x][y-1])+(potentials[x][y+1]-potentials[x][y]))/2.0)/MESH_SCALE_Y;
}

void display_potentials(float potentials[E_MESH_X][E_MESH_Y]){
  float max = float_array_max(potentials);
  float min = float_array_min(potentials);

  for(int y = 0; y < E_MESH_Y; y++){
    for(int x = 0; x < E_MESH_X; x++){
      int r = (potentials[x][y]/((max-min)/2.0))*255.0 * (float)(potentials[x][y] > 0);
      int g = (potentials[x][y]/((max-min)/2.0))*-255.0 * (float)(potentials[x][y] < 0);
      int b = 0;
      printf("\x1b[38;2;%i;%i;%im# ",r,g,b);
    }
    printf("\n");
  }
}

void clear_screen(){
  printf("\033[2J\033[1;1H");
}

int main(){

  float desired_gradients[E_MESH_X][E_MESH_Y] = {};
  bool desired_gradient_positions[E_MESH_X][E_MESH_Y] = {};

  float potentials[E_MESH_X][E_MESH_Y] = {};
  bool boundary_conditions[E_MESH_X][E_MESH_Y] = {};
  // potentials[25][25] = 100.0;
  // potentials[40][40] = 100.0;
  boundary_conditions[25][25] = 1;
  // boundary_conditions[40][40] = 1;

  relax_laplace_potentials(potentials,boundary_conditions,0.1);

  for(int y = 0; y < E_MESH_Y; y++){
    x_electric_field_at_position(potentials,25,y)
    y_electric_field_at_position(potentials,25,y)
  }
  display_potentials(potentials);
}
