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

#include <mpi.h>


#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <boost/array.hpp>

#include "data_structure.hpp"

// using namespace std::chrono;

#define EPSILON_0 8.854187e-12


#define MULTIGRID_COARSE_GRID 50


root_mesh_geometry::root_mesh_geometry(float bounds[6], float new_root_scale, float new_sub_scale){
  /*
  Creates a new mesh with the specified parameters.

  Root and sub_ scale are in meters per gridpoint.
  sub_scale will be rounded to the nearest integer fraction of root_scale.

  Mesh bounds will be rounded up to the nearest root_scale.
  */


  x_min_bound = bounds[X1];
  y_min_bound = bounds[Y1];
  z_min_bound = bounds[Z1];

  root_x_len = ceil((bounds[X2]-x_min_bound)/new_root_scale);
  root_y_len = ceil((bounds[Y2]-y_min_bound)/new_root_scale);
  root_z_len = ceil((bounds[Z2]-z_min_bound)/new_root_scale);

  x_max_bound = x_min_bound+(root_x_len*new_root_scale);
  y_max_bound = y_min_bound+(root_y_len*new_root_scale);
  z_max_bound = z_min_bound+(root_z_len*new_root_scale);

  root_scale = new_root_scale;

  sub_len = (root_scale/new_sub_scale);

  sub_scale = root_scale/sub_len;

  virtual_x_len = root_x_len * sub_len;
  virtual_y_len = root_y_len * sub_len;
  virtual_z_len = root_z_len * sub_len;
}



double scharge_efield(float beam_current, float beam_velocity, float beam_radius, float sample_radius){
    //Calculate the electric field at the edge of a beam
    //Eq. 44 at https://arxiv.org/pdf/1401.3951.pdf
    //returns one value: V/m
    //Beam velocity is in m/s

    return ((beam_current/(2.0*(M_PI)*EPSILON_0*beam_velocity)) * (sample_radius/pow(beam_radius,2.0)));
}

// float float_array_max(float array[ELECTRODE_FIELD_MESH_X][ELECTRODE_FIELD_MESH_Y]){
//   float max = 0;
//   for(int x = 0; x < ELECTRODE_FIELD_MESH_X; x++){
//     for(int y = 0; y < ELECTRODE_FIELD_MESH_Y; y++){
//       if(array[x][y] > max){
//         max = array[x][y];
//       }
//     }
//   }
//   return max;
// }
//
// float float_array_min(float array[ELECTRODE_FIELD_MESH_X][ELECTRODE_FIELD_MESH_Y]){
//   float min = 0;
//   for(int x = 0; x < ELECTRODE_FIELD_MESH_X; x++){
//     for(int y = 0; y < ELECTRODE_FIELD_MESH_Y; y++){
//         if(array[x][y] < min){
//           min = array[x][y];
//       }
//     }
//   }
//   return min;
// }

// float interpolated_field(x,y,z,potential,){
//
// }

int submesh_value(root_mesh_geometry mesh_geometry){
  return 0;

}

int root_submesh_index(int x, int y, int z, root_mesh_geometry mesh_geometry){
  /*
  Returns the submesh index of
  */
  return 0;
}



// int submesh_cube_length(int x, int y, int z, std::vector<std::vector<float>> root_mesh){
//   root_mesh[]
//   return pow(,(1.0/3.0));
// }



int relax_laplace_potentials(std::vector<float> &potentials_vector, std::vector<int> &boundary_conditions_vector, int x_len, int y_len, int z_len, float tolerance){
  /*
  For Jacobi, you store the new values in a new buffer; in Gauss-Seidel, you update them immediately.

  Jacobi is a bit slower, but allows parallel processing.

  In fact, using Jacobi and skipping the boundary condition check runs almost 3 times faster than
  Gauss-Seidel with a BC flag. The BCs are re-added to the new mesh after each iteration.

  I'm still not sure why this is; some cursory objdump was not enlightening.

  The solver sets the edges of the boundary to zero.

  Boundary conditions are included in the five-point star, but aren't modified.

  All input vectors must have the same dimensions.


  */


  // int world_rank;
  // MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  // int world_size;
  // MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // MPI_Send(&tosend[0], tosend_size, MPI_INT, 0, 777, MPI_COMM_WORLD); //vector mpi
  // results_and_rank.resize(results_rank_size);
  // MPI_Recv(&results_and_rank[0], results_rank_size, MPI_INT, MPI_ANY_SOURCE, 777, MPI_COMM_WORLD, &status);

  if(potentials_vector.size() != boundary_conditions_vector.size()){
    throw std::invalid_argument( "received negative value" );
  }


  printf("Solving mesh %i, %i, %i\n", x_len, y_len, z_len);

  if(x_len > 50 || y_len > 50 || z_len > 50){ //recursive coarse mesh solver
    int coarse_grid_len = (MULTIGRID_COARSE_GRID*MULTIGRID_COARSE_GRID*MULTIGRID_COARSE_GRID);
    std::vector<float> coarse_potential_vector(coarse_grid_len,0);
    std::vector<int> coarse_boundary_conditions_vector(coarse_grid_len,0);

    for(int i = 0; i < coarse_grid_len; i++){coarse_potential_vector[i] = potentials_vector[i*4];};
    for(int i = 0; i < coarse_grid_len; i++){coarse_boundary_conditions_vector[i] = boundary_conditions_vector[i*4];};

    relax_laplace_potentials(coarse_potential_vector,coarse_boundary_conditions_vector,
                                        MULTIGRID_COARSE_GRID,MULTIGRID_COARSE_GRID,MULTIGRID_COARSE_GRID,tolerance);
  }


  int total_mesh_len = (x_len*y_len*z_len);

  int xy_len = x_len*y_len;

  /*
  Allocate a few plain C-style arrays and cache-align for performance.
  */
  float * potentials;
  const size_t CACHE_LINE_SIZE = 256;
  posix_memalign( reinterpret_cast<void**>(&potentials), CACHE_LINE_SIZE, sizeof(float) * total_mesh_len);
  potentials = new float[total_mesh_len];
  memset(potentials, 0, total_mesh_len*sizeof(*potentials));
  for(int i = 0; i < total_mesh_len; i++){ potentials[i] = potentials_vector[i];};

  float * next_potentials;
  posix_memalign( reinterpret_cast<void**>(&next_potentials), CACHE_LINE_SIZE, sizeof(float) * total_mesh_len);
  next_potentials = new float[total_mesh_len];
  memset(next_potentials, 0, total_mesh_len*sizeof(*next_potentials));

  bool * boundaries;
  posix_memalign( reinterpret_cast<void**>(&boundaries), CACHE_LINE_SIZE, sizeof(bool) * total_mesh_len);
  boundaries = new bool[total_mesh_len];
  memset(boundaries, 0, total_mesh_len*sizeof(*boundaries));
  for(int i = 0; i < total_mesh_len; i++){ boundaries[i] = boundary_conditions_vector[i];};




  float new_convergence = 0;
  int iterations = 0;

  auto t1 = std::chrono::high_resolution_clock::now();

  for(iterations = 1; iterations < 10000; iterations++){

    for(int x = 1; x < x_len-1; x++){ //edges must be grounded.
      for(int y = 1; y < y_len-1; y++){
        for(int z = 1; z < z_len-1; z++){
          int position = (xy_len*z) + (x_len*y) + x;
          next_potentials[position] = (potentials[position + 1] +
                                       potentials[position - 1] +
                                       potentials[position-x_len] +
                                       potentials[position+x_len] +
                                       potentials[position-xy_len] +
                                       potentials[position+xy_len])/6.0;
        }
      }
    }

    for(int i = 0; i < total_mesh_len; i++){ //reset boundary conditions
      if(boundaries[i]){
        next_potentials[i] = potentials[i];
      }

      if(fabs(potentials[i]-next_potentials[i]) > new_convergence){
        new_convergence = fabs(potentials[i]-next_potentials[i]); //maximum difference between old and new
      }
      potentials[i] = next_potentials[i];
    }

    if(new_convergence < tolerance){ //exit condition
      break;
    }

    printf("convergence: %f\n",new_convergence);
    new_convergence = 0;
  }

  auto t2 = std::chrono::high_resolution_clock::now();

  std::cout << iterations << " iterations, " << (std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count())/iterations << " ms each" << "\n";
  std::cout << "total time " << (std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()) << " ms" << "\n";

  for(int i = 0; i < total_mesh_len; i++){ potentials_vector[i] = next_potentials[i];};
  delete[] potentials;
  delete[] boundaries;
  delete[] next_potentials;

  return iterations;
  // return potentials[0];
  //throw std::runtime_error("Laplace did not converge!");
}


void electric_field(std::vector<float> &potentials, int x, int y, int z, int mesh_geometry[3], float mesh_scale[3], float gradient[3]){
  /*
  Basic 3d field gradient computation.pa
  Returns 0 if position is too close to an edge.
  */
  if(x > mesh_geometry[X]-1 || y > mesh_geometry[Y]-1 || y > mesh_geometry[Y]-1){
    gradient[X] = 0;
    gradient[Y] = 0;
    gradient[Z] = 0;
  }
  else{
    gradient[X] = (potentials[i_idx(x+1,y,z,mesh_geometry)]-potentials[i_idx(x,y,z,mesh_geometry)])/mesh_scale[X]; //unsymmetric field measurement; I don't think this matters.
    gradient[Y] = (potentials[i_idx(x,y+1,z,mesh_geometry)]-potentials[i_idx(x,y,z,mesh_geometry)])/mesh_scale[Y];
    gradient[Z] = (potentials[i_idx(x,y,z+1,mesh_geometry)]-potentials[i_idx(x,y,z,mesh_geometry)])/mesh_scale[Z];
  }
}

void interpolated_electric_field(std::vector<float> &potentials, int x, int y, int z, int mesh_geometry[3], float mesh_scale[3], float gradient[3]){

}

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



int f_idx(float x, float y, float z, int mesh_geometry[3], float mesh_scale[3]){
  /*
  Helper function to obtain 1D mesh index from float 3D world position
  sanity checking should be done in caller for performance reasons
  */
  // if(x > mesh_geometry[X]){
  //   throw std::invalid_argument( "received negative value" );
  // }

  return (mesh_geometry[X]*mesh_geometry[Y]*(z/mesh_scale[Z])) + (mesh_geometry[X]*(y/mesh_scale[Y])) + x/mesh_scale[X];
}

int i_idx_2(int x, int y, int z, int x_len, int y_len, int z_len){
  /*
  Helper function to obtain 1D mesh index from int 3D mesh position
  sanity checking should be done in caller for performance reasons
  */
  return (x_len*y_len*z) + (x_len*y) + x;
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
