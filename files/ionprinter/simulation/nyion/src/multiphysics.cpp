#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <chrono>
#include <vector>

#include "multiphysics.hpp"
#include <numeric>

#include <stdexcept>

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

#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkCellArray.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkProperty.h>

#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>

#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkIntArray.h>

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

float interpolated_field(x,y,z,potential,){

}

void relax_laplace_potentials(std::vector<float> potentials, std::vector<bool> boundary_conditions, std::vector<bool> active,
                                                                                              int mesh_geometry[3], float tolerance){
  /*
  For Jacobi, you store the new values in a new buffer; in Gauss-Seidel, you update them immediately.

  Jacobi is a bit slower, but allows parallel processing.

  Boundary conditions are included in the five-point star, but aren't modified.



  All input vectors must have the same dimensions.

  */

  // MPI_Send(&tosend[0], tosend_size, MPI_INT, 0, 777, MPI_COMM_WORLD); //vector mpi
  // results_and_rank.resize(results_rank_size);
  // MPI_Recv(&results_and_rank[0], results_rank_size, MPI_INT, MPI_ANY_SOURCE, 777, MPI_COMM_WORLD, &status);

  if(potentials.size() != boundary_conditions.size() || boundary_conditions.size() != active.size()){
    throw std::invalid_argument( "received negative value" );
  }

  float previous_convergence = 0;
  for(int i = 0; i < 100000; i++){
    for(int x = 0; x < mesh_geometry[X]; x++){
      for(int y = 0; y < mesh_geometry[Y]; y++){
        for(int z = 0; z < mesh_geometry[Z]; z++){
          if(!boundary_conditions[i_idx(x,y,z,mesh_geometry)] && active[i_idx(x,y,z,mesh_geometry)]){ //check if the middle of the five-point star is a BC
            float potential_sum = 0;
            float potential_average_count = 0;
            if(x > 0 && active[i_idx(x-1,y,z,mesh_geometry)]){  //edge of domain or open edge of active region
                                                      //this sort of graceful edge handling is really slow
                                                          //it may be worthwhile to perform this checking in advance somehow
                potential_sum+=potentials[x-1];
                potential_average_count++;
            }
            if(x <  && active[i_idx(x-1,y,z,mesh_geometry)]){ //each point in the star is treated seperately
                                                                //which is pretty stupid but whatever
                potential_sum+=potentials[x+1];
                potential_average_count++;
            }
            if(x > 0 && active[i_idx(x-1,y,z,mesh_geometry)]){
                potential_sum+=potentials[x+1];
                potential_average_count++;
            }
          }
        }
      }
    }

    new_convergence = sqrt(std::inner_product(potentials.begin(), potentials.end(), potentials.begin(), 0 )/potentials.size());

    if(fabs(new_convergence-previous_convergence) < tolerance){ //simpler than the spectral radius metric
      return;
    }

    previous_convergence = new_convergence;

  }
  throw std::runtime_error("Laplace did not converge!");
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
  /*
  Helper function to obtain 1D mesh index from float 3D world position
  sanity checking should be done in caller for performance reasons
  */
  // if(x > mesh_geometry[X]){
  //   throw std::invalid_argument( "received negative value" );
  // }
  return (mesh_geometry[X]*mesh_geometry[Y]*(z/mesh_scale[Z])) + (mesh_geometry[X]*(y/mesh_scale[Y])) + x/mesh_scale[X];
}


int i_idx(int x, int y, int z, int mesh_geometry[3]){
  /*
  Helper function to obtain 1D mesh index from int 3D mesh position
  sanity checking should be done in caller for performance reasons
  */
  return (mesh_geometry[X]*mesh_geometry[Y]*z) + (mesh_geometry[X]*y) + x ;
}

void import_mesh(const char* filename, std::vector<bool> &mesh_present, int mesh_geometry[3], float mesh_scale[3], double bounds[6]){
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

  // for(int i = 0; i < 6; i++){bounds[i] += translate[i/2];}; //translate

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
        point[X] = (x*mesh_scale[X]);
        point[Y] = (y*mesh_scale[Y]);
        point[Z] = (z*mesh_scale[Z]);
        points->SetPoint(0,point);
        pointsPolydata->SetPoints(points);
        selectEnclosedPoints->Update();
        mesh_present[i_idx(x,y,z,mesh_geometry)] = selectEnclosedPoints->IsInside(0);
      }
    }
  }
}



void write_structured_grid_vtu(const char* filename, std::vector<bool> mesh, int mesh_geometry[3], float mesh_scale[3]){

  vtkSmartPointer<vtkPoints> points =
    vtkSmartPointer<vtkPoints>::New();
  points->InsertNextPoint (0.0, 0.0, 0.0);
  points->InsertNextPoint (1.0, 0.0, 0.0);
  points->InsertNextPoint (0.0, 1.0, 0.0);

  for ( unsigned int i = 0; i < 10; ++i )
  {
    points->InsertNextPoint ( i, i, i );
  }

  vtkSmartPointer<vtkFloatArray> floats =
    vtkSmartPointer<vtkFloatArray>::New();
  floats->SetName("Temperature");
  floats->SetNumberOfComponents(1);
  for(int i = 0; i < mesh.size(); i++){
    floats->InsertValue(i,(float) mesh[i]);
  }

  vtkSmartPointer<vtkPolyData> polydata =
    vtkSmartPointer<vtkPolyData>::New();
  polydata->SetPoints(points);

  polydata->GetPointData()->AddArray(floats);

  vtkSmartPointer<vtkXMLPolyDataWriter> writer =
  vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetFileName(filename);
  writer->SetInputData(polydata);
  writer->Write();

}
