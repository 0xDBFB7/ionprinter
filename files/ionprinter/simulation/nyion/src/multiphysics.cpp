#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <chrono>
#include <vector>

#include "multiphysics.hpp"

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

// using namespace std::chrono;

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
  /*
  For Jacobi, you store the new values in a new buffer; in Gauss-Seidel, you update them immediately.
  */
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
    if(tolerance > 0){

    }
    //new convergence algo:
    //euclidean norm of (this matrix - previous matrix ) / norm of this matrix   < tol
    // new_convergence = std::abs(potentials[x][y]);
  }
}

// float electric_field_at_position(float potentials[ELECTRODE_FIELD_MESH_X][ELECTRODE_FIELD_MESH_Y],int x,int y){
//   return (((potentials[x][y]-potentials[x-1][y])+(potentials[x+1][y]-potentials[x][y]))/2.0)/ELECTRODE_MESH_SCALE_X; //determine gradient along x, divide by world scale
// }

//
// void display_potentials(float potentials[ELECTRODE_FIELD_MESH_X][ELECTRODE_FIELD_MESH_Y]){
//   float max = float_array_max(potentials);
//   float min = float_array_min(potentials);
//
//   for(int y = 0; y < ELECTRODE_FIELD_MESH_Y; y++){
//     for(int x = 0; x < ELECTRODE_FIELD_MESH_X; x++){
//       int r = 0;
//       int g = 0;
//       int b = 0;
//       if(max-min != 0){
//         r = (potentials[x][y]/((max-min)/2.0))*255.0 * (float)(potentials[x][y] > 0);
//         g = (potentials[x][y]/((max-min)/2.0))*-255.0 * (float)(potentials[x][y] < 0);
//         b = 0;
//       }
//       printf("\x1b[38;2;%i;%i;%im# ",r,g,b);
//     }
//     printf("\n");
//   }
// }

// void clear_screen(){
//   printf("\033[2J\033[1;1H");
// }

int f_idx(float x, float y, float z,int mesh_geometry[3], float mesh_scale[3]){
  /* Helper function to obtain 1D mesh index from 3D position
  */
  // if(x > mesh_geometry[X]){
  //   throw std::invalid_argument( "received negative value" );
  // }
  return (mesh_geometry[X]*mesh_geometry[Y]*(z/mesh_scale[Z])) + (mesh_geometry[X]*(y/mesh_scale[Y])) + x/mesh_scale[X];
}


int i_idx(int x, int y, int z, int mesh_geometry[3]){
  /* Helper function to obtain 1D mesh index from 3D position
  */
  return (mesh_geometry[X]*mesh_geometry[Y]*z) + (mesh_geometry[X]*y) + x ;
}

void import_mesh(const char* filename, std::vector<bool> &mesh_present, int mesh_geometry[3], float mesh_scale[3], double bounds[6], float translate[3]){
  /*
  Deposit a mesh onto a uniform grid.
  Inputs are filename c_string, a 1-D mesh with length

  The convoluted mesh_geometry setup is used because 3d vectors have large overhead in C++
  mesh_geometry stores the dimensions of the mesh in indices, mesh scale stores grid to world scale in meters

  Outputs bounds of imported mesh.
  */
  vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
  reader->SetFileName(filename);
  reader->Update();

  vtkSmartPointer<vtkVoxelModeller> voxelModeller = vtkSmartPointer<vtkVoxelModeller>::New();
  reader->GetOutput()->GetBounds(bounds);
  for(int i = 0; i < 6; i++) bounds[i] /= 1000.0; //convert mm to m

  for(int i = 0; i < 6; i++){bounds[i] += translate[i/2];}; //translate

  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkPolyData> pointsPolydata = vtkSmartPointer<vtkPolyData>::New();
  //Points inside test
  vtkSmartPointer<vtkSelectEnclosedPoints> selectEnclosedPoints =
    vtkSmartPointer<vtkSelectEnclosedPoints>::New();
  selectEnclosedPoints->SetInputData(pointsPolydata);
  selectEnclosedPoints->SetSurfaceConnection(reader->GetOutputPort());

  double point[3] = {0, 0.0, 0.0};
  points->InsertNextPoint(point);

  // throw std::invalid_argument( "received negative value" );


  for(int x = std::max(bounds[0]/mesh_scale[X],0.0); x < std::min((int)(bounds[1]/mesh_scale[X]),mesh_geometry[X]); x++){
    for(int y = std::max(bounds[2]/mesh_scale[Y],0.0); y < std::min((int)(bounds[3]/mesh_scale[Y]),mesh_geometry[Y]); y++){
      for(int z = std::max(bounds[4]/mesh_scale[Z],0.0); z < std::min((int)(bounds[5]/mesh_scale[Z]),mesh_geometry[Z]); z++){
        point[X] = (x*mesh_scale[X])-translate[X];
        point[Y] = (y*mesh_scale[Y])-translate[Y];
        point[Z] = (z*mesh_scale[Z])-translate[Z];
        points->SetPoint(0,point);
        pointsPolydata->SetPoints(points);
        selectEnclosedPoints->Update();
        mesh_present[i_idx(x,y,z,mesh_geometry)] = selectEnclosedPoints->IsInside(0);
      }
    }
  }
}
