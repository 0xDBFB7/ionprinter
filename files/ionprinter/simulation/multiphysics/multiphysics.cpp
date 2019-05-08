#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <chrono>
#include "QUnit.hpp"

// #include "tiny_obj_loader.h"
#include <vtkVersion.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkCubeSource.h>
#include <vtkSmartPointer.h>
#include <vtkSelectEnclosedPoints.h>
#include <vtkIntArray.h>
#include <vtkDataArray.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyData.h>
#include <vtkSTLReader.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>
#include <vtkVoxelModeller.h>
#include <vtkPolyData.h>
#include <vtkXMLImageDataWriter.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

using namespace std::chrono;

#define BEAM_COUNT 5
#define BEAM_STEPS 100

#define EPSILON_0 8.854187e-12

#define MESH_X 0.1 //meters
#define MESH_Y 0.1
#define MESH_Z 0.1

#define ELECTRODE_FIELD_MESH_X 100
#define ELECTRODE_FIELD_MESH_Y 100
#define ELECTRODE_FIELD_MESH_Z 100

#define ELECTRODE_MESH_SCALE_X (MESH_X/ELECTRODE_FIELD_MESH_X) //meters
#define ELECTRODE_MESH_SCALE_Y (MESH_Y/ELECTRODE_FIELD_MESH_Y) //meters
#define ELECTRODE_MESH_SCALE_Z (MESH_Z/ELECTRODE_FIELD_MESH_Z) //meters

#define SPACE_CHARGE_FIELD_MESH_X 100
#define SPACE_CHARGE_FIELD_MESH_Y 100
#define SPACE_CHARGE_FIELD_MESH_Z 100

#define THERMAL_FIELD_MESH_X 1000
#define THERMAL_FIELD_MESH_Y 1000
#define THERMAL_FIELD_MESH_Z 1000
#define THERMAL_MESH_SCALE_X (MESH_X/THERMAL_FIELD_MESH_X) //meters
#define THERMAL_MESH_SCALE_Y (MESH_Y/THERMAL_FIELD_MESH_Y)
#define THERMAL_MESH_SCALE_Z (MESH_Z/THERMAL_FIELD_MESH_Z)


#define X 0
#define Y 1
#define Z 2

bool mesh_present[THERMAL_FIELD_MESH_X][THERMAL_FIELD_MESH_Y][THERMAL_FIELD_MESH_Z] = {};


double scharge_efield(float beam_current, float beam_velocity, float beam_radius, float sample_radius){
    //Calculate the electric field at the edge of a beam
    //Eq. 44 at https://arxiv.org/pdf/1401.3951.pdf
    //returns one value: V/m
    //Beam velocity is in m/s

    return ((beam_current/(2.0*(M_PI)*EPSILON_0*beam_velocity)) * (sample_radius/pow(beam_radius,2.0)));
}

float float_array_max(float array[ELECTRODE_FIELD_MESH_X][ELECTRODE_FIELD_MESH_Y]){
  float max = 0;
  for(int x = 0; x < ELECTRODE_FIELD_MESH_X; x++){
    for(int y = 0; y < ELECTRODE_FIELD_MESH_Y; y++){
      if(array[x][y] > max){
        max = array[x][y];
      }
    }
  }
  return max;
}

float float_array_min(float array[ELECTRODE_FIELD_MESH_X][ELECTRODE_FIELD_MESH_Y]){
  float min = 0;
  for(int x = 0; x < ELECTRODE_FIELD_MESH_X; x++){
    for(int y = 0; y < ELECTRODE_FIELD_MESH_Y; y++){
        if(array[x][y] < min){
          min = array[x][y];
      }
    }
  }
  return min;
}

void relax_laplace_potentials(float potentials[ELECTRODE_FIELD_MESH_X][ELECTRODE_FIELD_MESH_Y], bool boundary_conditions[ELECTRODE_FIELD_MESH_X][ELECTRODE_FIELD_MESH_Y], float tolerance){
  // int convergence_index_x = 0;//Find a point to sample for convergence
  // int convergence_index_y = 0;

  // for(int x = 1; x < ELECTRODE_FIELD_MESH_X-1; x++){
  //   for(int y = 1; y < ELECTRODE_FIELD_MESH_Y-1; y++){
  //     if(boundary_conditions[x][y]){

  for(int i = 0; i < 3000; i++){
    for(int x = 1; x < ELECTRODE_FIELD_MESH_X-1; x++){
      for(int y = 1; y < ELECTRODE_FIELD_MESH_Y-1; y++){
        if(!boundary_conditions[x][y]){
          potentials[x][y] = (potentials[x-1][y]+potentials[x+1][y]+potentials[x][y+1]+potentials[x][y-1])/4.0;
          // printf("%f\n",potentials[x][y]);
        }
      }
    }
    // new_convergence = std::abs(potentials[x][y]);
  }
}

float x_electric_field_at_position(float potentials[ELECTRODE_FIELD_MESH_X][ELECTRODE_FIELD_MESH_Y],int x,int y){
  return (((potentials[x][y]-potentials[x-1][y])+(potentials[x+1][y]-potentials[x][y]))/2.0)/ELECTRODE_MESH_SCALE_X; //determine gradient along x, divide by world scale
}

float y_electric_field_at_position(float potentials[ELECTRODE_FIELD_MESH_X][ELECTRODE_FIELD_MESH_Y],int x,int y){
  return (((potentials[x][y]-potentials[x][y-1])+(potentials[x][y+1]-potentials[x][y]))/2.0)/ELECTRODE_MESH_SCALE_Y; //determine gradient along y, divide by world scale
}

void display_potentials(float potentials[ELECTRODE_FIELD_MESH_X][ELECTRODE_FIELD_MESH_Y]){
  float max = float_array_max(potentials);
  float min = float_array_min(potentials);

  for(int y = 0; y < ELECTRODE_FIELD_MESH_Y; y++){
    for(int x = 0; x < ELECTRODE_FIELD_MESH_X; x++){
      int r = 0;
      int g = 0;
      int b = 0;
      if(max-min != 0){
        r = (potentials[x][y]/((max-min)/2.0))*255.0 * (float)(potentials[x][y] > 0);
        g = (potentials[x][y]/((max-min)/2.0))*-255.0 * (float)(potentials[x][y] < 0);
        b = 0;
      }
      printf("\x1b[38;2;%i;%i;%im# ",r,g,b);
    }
    printf("\n");
  }
}

void clear_screen(){
  printf("\033[2J\033[1;1H");
}

float gradient_difference(float desired_gradients[2][ELECTRODE_FIELD_MESH_X][ELECTRODE_FIELD_MESH_Y], bool desired_gradient_positions[2][ELECTRODE_FIELD_MESH_X][ELECTRODE_FIELD_MESH_Y], float potentials[ELECTRODE_FIELD_MESH_X][ELECTRODE_FIELD_MESH_Y]){
  float sum = 0;
  for(int y = 1; y < ELECTRODE_FIELD_MESH_Y-1; y++){
    for(int x = 1; x < ELECTRODE_FIELD_MESH_X-1; x++){
      if(desired_gradient_positions[X][x][y]){
        sum += std::abs(desired_gradients[X][x][y]-x_electric_field_at_position(potentials,x,y));
      }
      if(desired_gradient_positions[Y][x][y]){ //perhaps the Y field can be free while the X field is fixed
        sum += std::abs(desired_gradients[Y][x][y]-y_electric_field_at_position(potentials,x,y));
      }
    }
  }
  return sum;
}


void import_mesh(bool mesh_present[THERMAL_FIELD_MESH_X][THERMAL_FIELD_MESH_Y][THERMAL_FIELD_MESH_Z]){

    vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName("test.stl");
    reader->Update();

    vtkSmartPointer<vtkVoxelModeller> voxelModeller = vtkSmartPointer<vtkVoxelModeller>::New();
    double bounds[6];
    reader->GetOutput()->GetBounds(bounds);

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPolyData> pointsPolydata = vtkSmartPointer<vtkPolyData>::New();
    //Points inside test
    vtkSmartPointer<vtkSelectEnclosedPoints> selectEnclosedPoints =
      vtkSmartPointer<vtkSelectEnclosedPoints>::New();
    selectEnclosedPoints->SetInputData(pointsPolydata);
    selectEnclosedPoints->SetSurfaceConnection(reader->GetOutputPort());

    double point[3] = {0, 0.0, 0.0};
    points->InsertNextPoint(point);
    printf("bounds: %f, %f\n",bounds[0],bounds[1]);
    printf("bounds: %f, %f\n",std::max((bounds[0]/1000.0)/THERMAL_MESH_SCALE_X,0.0),std::min(((bounds[1]/1000.0)/THERMAL_MESH_SCALE_X),MESH_X/THERMAL_MESH_SCALE_X));

    for(int x = std::max((bounds[0]/1000.0)/THERMAL_MESH_SCALE_X,0.0); x < std::min(((bounds[1]/1000.0)/THERMAL_MESH_SCALE_X),MESH_X/THERMAL_MESH_SCALE_X); x++){
      for(int y = std::max((bounds[2]/1000.0)/THERMAL_MESH_SCALE_Y,0.0); y < std::min(((bounds[3]/1000.0)/THERMAL_MESH_SCALE_Y),MESH_Y/THERMAL_MESH_SCALE_Y); y++){
        for(int z = std::max((bounds[4]/1000.0)/THERMAL_MESH_SCALE_Z,0.0); z < std::min(((bounds[5]/1000.0)/THERMAL_MESH_SCALE_Z),MESH_Z/THERMAL_MESH_SCALE_Z); z++){
          point[X] = (x*THERMAL_MESH_SCALE_X)*1000.0; //multiply by world mesh scale, multiply by 1000.
          point[Y] = (y*THERMAL_MESH_SCALE_Y)*1000.0;
          point[Z] = (z*THERMAL_MESH_SCALE_Z)*1000.0;
          points->SetPoint(0,point);
          pointsPolydata->SetPoints(points);
          selectEnclosedPoints->Update();
          mesh_present[x][y][z] = selectEnclosedPoints->IsInside(0);
          // printf("");
          // printf("x%i,y%i\n",x,y);
        }
      }
    }
    //

    //
    // double bounds[6];
    // pointsPolydata->GetBounds(bounds);
    // std::cout  << "xmin: " << bounds[0] << " "
    //            << "xmax: " << bounds[1] << std::endl
    //            << "ymin: " << bounds[2] << " "
    //            << "ymax: " << bounds[3] << std::endl
    //            << "zmin: " << bounds[4] << " "
    //            << "zmax: " << bounds[5] << std::endl;
    //
    // for(int x = 0; x < bounds[0]; x++){
    //   for(int y = 0; y < bounds[0]; y++){
    //     for(int z = 0; z < bounds[0]; z++){
    //
    //     }
    //   }
    // }
}

int main(){
  import_mesh(mesh_present);

  //
  // float beam_diagnostics[BEAM_COUNT][10][] = {};
  //
  // float potentials[ELECTRODE_FIELD_MESH_X][ELECTRODE_FIELD_MESH_Y] = {};
  // bool boundary_conditions[ELECTRODE_FIELD_MESH_X][ELECTRODE_FIELD_MESH_Y] = {};
  //
  //
  // potentials[x][y] = v;
  // boundary_conditions[x][y] = 1;
  // auto start = high_resolution_clock::now();
  //
  // relax_laplace_potentials(potentials,boundary_conditions,0.1);
  //
  // auto stop = high_resolution_clock::now();
  // auto duration = duration_cast<microseconds>(stop - start);
  //
  // particle_position[2] = {0,0};
  // particle_velocity[2] = {2000,0};
  //
  // for(int beam_index = 0; beam_index < BEAM_COUNT; beam_index++){
  //   for(int beam_step = 0; beam_step < BEAM_STEPS; beam_step++){
  //
  //   }
  // }
  // // printf(lowest_diff)
  // // printf("%i%% complete\n",(int) (y/ELECTRODE_FIELD_MESH_Y)*100);
  // clear_screen();
  // printf("\x1b[38;2;255;255;0mDan's Hyper Beam Solver\n");
  // printf("\x1b[38;2;255;255;255mcur: %f, lowest: %f, v: %f x: %i y: %i\n",diff,lowest_diff,v,x,y);
  // std::cout << "Time taken by function: " << duration.count() << " microseconds\n";


    // display_potentials(potentials);
    // display_potentials(potentials);
  // display_potentials(potentials);
}
