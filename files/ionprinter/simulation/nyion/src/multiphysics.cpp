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
#include <math.h>
// using namespace std::chrono;

#define EPSILON_0 8.854187e-12


#define MULTIGRID_COARSE_GRID 0.001


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

  root_size = root_x_len*root_y_len*root_z_len;

  x_max_bound = x_min_bound+(root_x_len*new_root_scale);
  y_max_bound = y_min_bound+(root_y_len*new_root_scale);
  z_max_bound = z_min_bound+(root_z_len*new_root_scale);

  root_scale = new_root_scale;

  sub_len = (root_scale/new_sub_scale);

  sub_scale = root_scale/sub_len;
  sub_size = (sub_len*sub_len*sub_len);

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


template<typename T>
void enable_mesh_region(std::vector<std::vector<T>> &mesh, float bounds[6], root_mesh_geometry mesh_geometry){

  int x_min = std::max(0,(int)((bounds[X1]-mesh_geometry.x_min_bound)/mesh_geometry.root_scale));
  int x_max = std::min(mesh_geometry.root_x_len,(int)((bounds[X2]-mesh_geometry.x_min_bound)/mesh_geometry.root_scale));
  int y_min = std::max(0,(int)((bounds[Y1]-mesh_geometry.y_min_bound)/mesh_geometry.root_scale));
  int y_max = std::min(mesh_geometry.root_y_len,(int)((bounds[Y2]-mesh_geometry.y_min_bound)/mesh_geometry.root_scale));
  int z_min = std::max(0,(int)((bounds[Z1]-mesh_geometry.z_min_bound)/mesh_geometry.root_scale));
  int z_max = std::min(mesh_geometry.root_z_len,(int)((bounds[Z2]-mesh_geometry.z_min_bound)/mesh_geometry.root_scale));


  for(int x = x_min; x < x_max; x++){
    for(int y = y_min; y < y_max; y++){
      for(int z = z_min; z < z_max; z++){
        mesh[idx_from_position(x,y,z,
                                mesh_geometry.root_x_len,
                                mesh_geometry.root_y_len,
                                mesh_geometry.root_z_len)].resize(mesh_geometry.sub_size);
      }
    }
  }
}
template void enable_mesh_region(std::vector<std::vector<int>> &mesh, float bounds[6], root_mesh_geometry mesh_geometry);
template void enable_mesh_region(std::vector<std::vector<float>> &mesh, float bounds[6], root_mesh_geometry mesh_geometry);


//
// float fast_get_mesh_value(int root_x, int root_y, int root_z, int sub_x, int sub_y, int sub_z,
//                                 std::vector<std::vector<float>> &mesh, root_mesh_geometry mesh_geometry){



template<typename T>
float get_mesh_value(int v_x, int v_y, int v_z, std::vector<std::vector<T>> &mesh, root_mesh_geometry mesh_geometry){
  int root_mesh_x = v_x/mesh_geometry.sub_len;
  int root_mesh_y = v_y/mesh_geometry.sub_len;
  int root_mesh_z = v_z/mesh_geometry.sub_len;

  if(root_mesh_x < 0 || root_mesh_y < 0 || root_mesh_z < 0
                    || root_mesh_x >= mesh_geometry.root_x_len
                    || root_mesh_y >= mesh_geometry.root_y_len
                    || root_mesh_z >= mesh_geometry.root_z_len){
    return 0.0;
  }

  int root_idx = ((mesh_geometry.root_x_len*mesh_geometry.root_y_len)*root_mesh_z) + ((mesh_geometry.root_x_len)*root_mesh_y) + root_mesh_x;

  int sub_mesh_x = v_x-(root_mesh_x*mesh_geometry.sub_len);
  int sub_mesh_y = v_y-(root_mesh_y*mesh_geometry.sub_len);
  int sub_mesh_z = v_z-(root_mesh_z*mesh_geometry.sub_len);

  if(sub_mesh_x < 0 || sub_mesh_y < 0 || sub_mesh_z < 0
                    ||sub_mesh_x >= mesh_geometry.sub_len
                    || sub_mesh_y >= mesh_geometry.sub_len
                    || sub_mesh_z >= mesh_geometry.sub_len){
    return 0.0;
  }

  int sub_idx = ((mesh_geometry.sub_len*mesh_geometry.sub_len)*sub_mesh_z) + ((mesh_geometry.sub_len)*sub_mesh_y) + sub_mesh_x;

  if(mesh[root_idx].size() == 0){
    return 0.0;
  }

  return mesh[root_idx][sub_idx];
}
template float get_mesh_value(int v_x, int v_y, int v_z, std::vector<std::vector<float>> &mesh, root_mesh_geometry mesh_geometry);
template float get_mesh_value(int v_x, int v_y, int v_z, std::vector<std::vector<int>> &mesh, root_mesh_geometry mesh_geometry);



template<typename T>
float get_mesh_value_world_point(float x, float y, float z, std::vector<std::vector<T>> &mesh, root_mesh_geometry mesh_geometry){

  // int root_x = x/mesh_geometry.root_scale;
  // int root_y = y/mesh_geometry.root_scale;
  // int root_z = z/mesh_geometry.root_scale;
  //
  // int root_idx = idx_from_position(root_x,root_y,root_z,mesh_geometry.root_x_len,mesh_geometry.root_y_len,mesh_geometry.root_z_len);
  //
  // int sub_x = x-(root_x*mesh_geometry.root_scale);
  // int sub_y = y-(root_y*mesh_geometry.root_scale);
  // int sub_z = z-(root_z*mesh_geometry.root_scale);
  //
  // int sub_idx = idx_from_position(root_x,root_y,root_z,mesh_geometry.root_x_len,mesh_geometry.root_y_len,mesh_geometry.root_z_len);


  return get_mesh_value(x/mesh_geometry.sub_scale,y/mesh_geometry.sub_scale,z/mesh_geometry.sub_scale,
                                      mesh, mesh_geometry);
}
template float get_mesh_value_world_point(float x, float y, float z, std::vector<std::vector<float>> &mesh, root_mesh_geometry mesh_geometry);
template float get_mesh_value_world_point(float x, float y, float z, std::vector<std::vector<int>> &mesh, root_mesh_geometry mesh_geometry);


template<typename T>
void set_mesh_value(float val, int v_x, int v_y, int v_z, std::vector<std::vector<T>> &mesh, root_mesh_geometry mesh_geometry){ // this is SO UGLY
  int root_mesh_x = v_x/mesh_geometry.sub_len;
  int root_mesh_y = v_y/mesh_geometry.sub_len;
  int root_mesh_z = v_z/mesh_geometry.sub_len;

  if(root_mesh_x < 0 || root_mesh_y < 0 || root_mesh_z < 0
                    || root_mesh_x >= mesh_geometry.root_x_len
                    || root_mesh_y >= mesh_geometry.root_y_len
                    || root_mesh_z >= mesh_geometry.root_z_len){
    return;
  }

  int root_idx = ((mesh_geometry.root_x_len*mesh_geometry.root_y_len)*root_mesh_z) + ((mesh_geometry.root_x_len)*root_mesh_y) + root_mesh_x;

  int sub_mesh_x = v_x-(root_mesh_x*mesh_geometry.sub_len);
  int sub_mesh_y = v_y-(root_mesh_y*mesh_geometry.sub_len);
  int sub_mesh_z = v_z-(root_mesh_z*mesh_geometry.sub_len);

  if(sub_mesh_x < 0 || sub_mesh_y < 0 || sub_mesh_z < 0
                    ||sub_mesh_x >= mesh_geometry.sub_len
                    || sub_mesh_y >= mesh_geometry.sub_len
                    || sub_mesh_z >= mesh_geometry.sub_len){
    return;
  }

  int sub_idx = ((mesh_geometry.sub_len*mesh_geometry.sub_len)*sub_mesh_z) + ((mesh_geometry.sub_len)*sub_mesh_y) + sub_mesh_x;

  if(mesh[root_idx].size() == 0){
    return;
  }

  mesh[root_idx][sub_idx] = val;
}
template void set_mesh_value(float val, int v_x, int v_y, int v_z, std::vector<std::vector<float>> &mesh, root_mesh_geometry mesh_geometry);
template void set_mesh_value(float val, int v_x, int v_y, int v_z, std::vector<std::vector<int>> &mesh, root_mesh_geometry mesh_geometry);



template<typename T>
void set_mesh_value_world_point(float val, float x, float y, float z, std::vector<std::vector<T>> &mesh, root_mesh_geometry mesh_geometry){

  set_mesh_value(val, x/mesh_geometry.sub_scale,y/mesh_geometry.sub_scale,z/mesh_geometry.sub_scale,
                                      mesh, mesh_geometry);
}
template void set_mesh_value_world_point(float val, float x, float y, float z, std::vector<std::vector<float>> &mesh, root_mesh_geometry mesh_geometry);
template void set_mesh_value_world_point(float val, float x, float y, float z, std::vector<std::vector<int>> &mesh, root_mesh_geometry mesh_geometry);





template<typename T>
root_mesh_geometry coarsen_mesh(std::vector<std::vector<T>> &original, std::vector<std::vector<T>> &coarsened, root_mesh_geometry original_geometry, float new_sub_scale){
  /*
  The root mesh resolution remains the same; only the submeshes are coarsened.
  */
  coarsened = original;

  float mesh_bounds[6] = {original_geometry.x_min_bound,
                            original_geometry.x_max_bound,
                            original_geometry.y_min_bound,
                            original_geometry.y_max_bound,
                            original_geometry.z_min_bound,
                            original_geometry.z_max_bound};

  root_mesh_geometry new_geometry(mesh_bounds, original_geometry.root_scale, new_sub_scale);

  int scale_divisor = new_geometry.sub_scale/original_geometry.sub_scale;

  for(int root_mesh_idx = 0; root_mesh_idx < original.size(); root_mesh_idx++){ //iterate over coarse submesh cubes
    if(original[root_mesh_idx].size()){
      coarsened[root_mesh_idx].resize(new_geometry.sub_size);

      int root_x;
      int root_y;
      int root_z;
      position_from_index(root_x,root_y,root_z,root_mesh_idx,new_geometry.root_x_len,new_geometry.root_y_len,new_geometry.root_z_len);

      for(int x = (root_x*new_geometry.sub_len); x < (root_x*new_geometry.sub_len)+new_geometry.sub_len; x++){
        for(int y = (root_y*new_geometry.sub_len); y < (root_y*new_geometry.sub_len)+new_geometry.sub_len; y++){
          for(int z = (root_z*new_geometry.sub_len); z < (root_z*new_geometry.sub_len)+new_geometry.sub_len; z++){
            set_mesh_value(get_mesh_value(x*scale_divisor,y*scale_divisor,z*scale_divisor,original,original_geometry),x,y,z,coarsened,new_geometry);
          }
        }
      }
    }
  }

  return new_geometry;
}
template root_mesh_geometry coarsen_mesh(std::vector<std::vector<float>> &original, std::vector<std::vector<float>> &coarsened, root_mesh_geometry original_geometry,float new_sub_scale);
template root_mesh_geometry coarsen_mesh(std::vector<std::vector<int>> &original, std::vector<std::vector<int>> &coarsened, root_mesh_geometry original_geometry,float new_sub_scale);


template<typename T>
root_mesh_geometry decoarsen_mesh(std::vector<std::vector<T>> &decoarsened, std::vector<std::vector<T>> &coarsened, root_mesh_geometry decoarsened_geometry, root_mesh_geometry coarse_geometry){

  int scale_divisor = decoarsened_geometry.sub_len/coarse_geometry.sub_len;

  for(int root_mesh_idx = 0; root_mesh_idx < decoarsened.size(); root_mesh_idx++){ //iterate over coarse submesh cubes

    if(coarsened[root_mesh_idx].size()){

      int root_x;
      int root_y;
      int root_z;
      position_from_index(root_x,root_y,root_z,root_mesh_idx,decoarsened_geometry.root_x_len,decoarsened_geometry.root_y_len,decoarsened_geometry.root_z_len);

      for(int x = (root_x*decoarsened_geometry.sub_len); x < (root_x*decoarsened_geometry.sub_len)+decoarsened_geometry.sub_len; x++){
        for(int y = (root_y*decoarsened_geometry.sub_len); y < (root_y*decoarsened_geometry.sub_len)+decoarsened_geometry.sub_len; y++){
          for(int z = (root_z*decoarsened_geometry.sub_len); z < (root_z*decoarsened_geometry.sub_len)+decoarsened_geometry.sub_len; z++){
            set_mesh_value(get_mesh_value(x/scale_divisor,y/scale_divisor,z/scale_divisor,coarsened,coarse_geometry),x,y,z,decoarsened,decoarsened_geometry);
          }
        }
      }
    }
  }
}
template root_mesh_geometry decoarsen_mesh(std::vector<std::vector<int>> &decoarsened, std::vector<std::vector<int>> &coarsened, root_mesh_geometry decoarsened_geometry, root_mesh_geometry coarse_geometry);
template root_mesh_geometry decoarsen_mesh(std::vector<std::vector<float>> &decoarsened, std::vector<std::vector<float>> &coarsened, root_mesh_geometry decoarsened_geometry, root_mesh_geometry coarse_geometry);






int fast_relax_laplace_potentials(std::vector<std::vector<int>> potentials_vector, std::vector<std::vector<int>> boundaries_vector, float tolerance){

  // int world_rank;
  // MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  // int world_size;
  // MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // MPI_Send(&tosend[0], tosend_size, MPI_INT, 0, 777, MPI_COMM_WORLD); //vector mpi
  // results_and_rank.resize(results_rank_size);
  // MPI_Recv(&results_and_rank[0], results_rank_size, MPI_INT, MPI_ANY_SOURCE, 777, MPI_COMM_WORLD, &status);


  int root_x = 200;
  int root_y = 200;
  int root_z = 200;
  int root_size = (root_x*root_y*root_z);
  int root_xy = (root_x * root_y);

  int * submesh_side_lengths = new int[root_size];
  std::fill(submesh_side_lengths, submesh_side_lengths + sizeof(submesh_side_lengths), 0);

  int **potentials = new int*[root_size];
  int **potentials_copy = new int*[root_size];
  int **boundaries = new int*[root_size];

  for(int root = 0; root < potentials_vector.size(); root++){
    submesh_side_lengths[root] = pow(potentials_vector[root].size(),1.0/3.0); //cube root
    int this_submesh_size = (submesh_side_lengths[root]*submesh_side_lengths[root]*submesh_side_lengths[root]);
    potentials[i] = new int[this_submesh_size];
    potentials_copy[i] = new int[this_submesh_size];
    boundaries[i] = new int[this_submesh_size];
    for(int sub = 0; sub < ; sub++){

  }


  for(int i = 0; i < 2; i++){
    for(int x = 0; x < submesh_side_lengths[i]; x++){
      potentials[i][x] = 1000000;
      boundaries[i][x] = 1;
    }
  }

  auto t1 = std::chrono::high_resolution_clock::now();

  int iterations = 0;

  int new_convergence = 0;

  for(iterations = 1; iterations < 100; iterations++){

    for(int root = 0; root < root_size; root++){ //re-add boundaries - saves the additional check
      if(submesh_side_lengths[root]){
        for(int sub = 0; sub < submesh_side_lengths[root]; sub++){
          potentials_copy[root][sub] = potentials[root][sub];
        }
      }
    }

    for(int root = 0; root < root_size; root++){
      if(submesh_side_lengths[root]){

        int this_submesh_side_length = submesh_side_lengths[root];
        int this_submesh_side_length_squared = this_submesh_side_length*this_submesh_side_length;
        int * this_potential_submesh = potentials[root]; //problematic?


        // int r_x = root % root_mesh_x; //ugly and slow! Dumb and stupid!
        // int r_y = (root / root_mesh_x) % root_mesh_y;
        // int r_z = root / (root_xy);

        // (x_len*y_len*z) + (x_len*y) + x;

        //for a typical 60^3 submesh, this loop will only run 0.0166x as many points as
        //the full submesh
        //so optimization isn't that crucial here
        for(int x = 0; x < this_submesh_side_length; x++){ //first do the edges, including the boundary check
          for(int y = 0; y < this_submesh_side_length; y++){ // this could be made even more efficient by doing a side at a time
            for(int z = 0; z < this_submesh_side_length; z++){


              int sub_idx = (this_submesh_side_length_squared*z) + (this_submesh_side_length*y) + x;


              if(!boundaries[root][sub_idx]){

                if(x == 0 || y == 0 || z == 0 || x == this_submesh_side_length //if we're at an edge or corner
                                              || y == this_submesh_side_length
                                              || z == this_submesh_side_length){ //not quite as fast as 6 seperate loops, but that's tedious and boring

                  int stencil_value = 0;

                  if(x == 0){ //"below": -z
                    if(root-1 > 0 && submesh_side_lengths[root-1]){
                      stencil_value += potentials[root-1]
                      [((submesh_side_lengths[root-1]*submesh_side_lengths[root-1])*z)
                            + (submesh_side_lengths[root-1]*submesh_side_lengths[root-1]) + 0];
                    }
                    //else add zero.
                  }
                  else{
                    stencil_value += this_potential_submesh[sub_idx-1];
                  }

                  if(x == this_submesh_side_length){ //"below": -z
                    if(root+1 > 0 && submesh_side_lengths[root+1]){
                      stencil_value += potentials[root+1]
                      [((submesh_side_lengths[root+1]*submesh_side_lengths[root+1])*z)
                            + (submesh_side_lengths[root+1]*y) + submesh_side_lengths[root+1]];
                    }
                    //else add zero.
                  }
                  else{
                    stencil_value += this_potential_submesh[sub_idx+1];
                  }

                  if(y == 0){ //"below": -z
                    if(root-root_y > 0 && submesh_side_lengths[root-root_y]){
                      stencil_value += potentials[root-root_y]
                      [((submesh_side_lengths[root-root_y]*submesh_side_lengths[root-root_y])*z)
                            + (submesh_side_lengths[root-root_y]*submesh_side_lengths[root-root_y]) + x];
                    }
                    //else add zero.
                  }
                  else{
                    stencil_value += this_potential_submesh[sub_idx-this_submesh_side_length];
                  }

                  if(y == this_submesh_side_length){ //"below": -z
                    if(root+root_y > 0 && submesh_side_lengths[root+root_y]){
                      stencil_value += potentials[root+root_y]
                      [((submesh_side_lengths[root+root_y]*submesh_side_lengths[root+root_y])*z)
                            + (submesh_side_lengths[root+root_y]*0) + x];
                    }
                    //else add zero.
                  }
                  else{
                    stencil_value += this_potential_submesh[sub_idx+this_submesh_side_length];
                  }

                  if(z == 0){ //"below": -z
                    if(root-root_xy > 0 && submesh_side_lengths[root-root_xy]){
                      stencil_value += potentials[root-root_xy]
                      [((submesh_side_lengths[root-root_xy]*submesh_side_lengths[root-root_xy])*submesh_side_lengths[root-root_xy])
                            + (submesh_side_lengths[root-root_xy]*y) + x];
                    }
                    //else add zero.
                  }
                  else{
                    stencil_value += this_potential_submesh[sub_idx-this_submesh_side_length_squared];
                  }

                  if(z == this_submesh_side_length){ //"above": +z
                    if(root+root_xy < root_size && submesh_side_lengths[root+root_xy]){
                      stencil_value += potentials[root+root_xy]
                      [((submesh_side_lengths[root+root_xy]*submesh_side_lengths[root+root_xy])*0) //z=0 on cell above
                            + (submesh_side_lengths[root+root_xy]*y) + x];
                    }
                    //else add zero.
                  }
                  else{
                    stencil_value += this_potential_submesh[sub_idx+this_submesh_side_length_squared];
                  }


                  stencil_value /= 6;
                  potentials[root][sub_idx] = stencil_value;
                }
                else{
                  potentials[root][sub_idx] =       (this_potential_submesh[sub_idx-1] +
                                                     this_potential_submesh[sub_idx+1] +
                                                     this_potential_submesh[sub_idx-this_submesh_side_length] +
                                                     this_potential_submesh[sub_idx+this_submesh_side_length] +
                                                     this_potential_submesh[sub_idx-this_submesh_side_length_squared] +
                                                     this_potential_submesh[sub_idx+this_submesh_side_length_squared])/6;
                }
              }
            }
          }
        }
      }
    }


    for(int root = 0; root < root_size; root++){ //re-add boundaries - saves the additional check
      if(submesh_side_lengths[root]){
        for(int sub = 0; sub < submesh_side_lengths[root]; sub++){

          // if(boundaries[root][sub]){
          //   potentials[root][sub] = potentials_copy[root][sub];
          // }

          if(fabs(potentials[root][sub]-potentials_copy[root][sub]) > new_convergence){
            new_convergence = fabs(potentials[root][sub]-potentials_copy[root][sub]); //maximum difference between old and new
          }


        }
      }
    }



    printf("convergence: %i\n",new_convergence);
    new_convergence = 0;

  }

  auto t2 = std::chrono::high_resolution_clock::now();

  for(int root = 0; root < root_size; root++){
    if(submesh_side_lengths[root]){
      delete[] potentials[root];
      delete[] boundaries[root];
      delete[] potentials_copy[root];
    }
  }


  delete[] submesh_side_lengths;
  delete[] boundaries;
  delete[] potentials;
  delete[] potentials_copy;



  std::cout << iterations << " iterations, " << (std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count())/(float) iterations << " ms each" << "\n";
  std::cout << "total time " << (std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()) << " ms" << "\n";
}





















int relax_laplace_potentials(std::vector<std::vector<float>> &potentials,
                            std::vector<std::vector<int>> &boundary_conditions, root_mesh_geometry mesh_geometry, float tolerance, bool root){
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

  if(potentials.size() != boundary_conditions.size()){
    throw std::invalid_argument("Input vectors have different dimensions.");
  }

  float max_value = 0;

  for(int root_mesh_idx = 0; root_mesh_idx < potentials.size(); root_mesh_idx++){ //iterate over coarse submesh cubes
    for(int submesh_idx = 0; submesh_idx < potentials[root_mesh_idx].size(); submesh_idx++){
      if(fabs(potentials[root_mesh_idx][submesh_idx]) > max_value){
        max_value = fabs(potentials[root_mesh_idx][submesh_idx]); //maximum difference between old and new
      }
    }
  }


  if(root){ //recursive coarse mesh solver

    for(int i = 10; i > 2; i-=1){
      std::vector<std::vector<float>> coarsened_potentials;
      std::vector<std::vector<int>> coarsened_boundaries;

      coarsened_potentials.resize(0);
      coarsened_boundaries.resize(0);

      root_mesh_geometry new_geometry = coarsen_mesh(potentials,coarsened_potentials,mesh_geometry,mesh_geometry.sub_scale*(i/2.0));
      coarsen_mesh(boundary_conditions,coarsened_boundaries,mesh_geometry,mesh_geometry.sub_scale*(i/2.0));

      relax_laplace_potentials(coarsened_potentials,coarsened_boundaries,new_geometry,0.01,0);

      decoarsen_mesh(potentials,coarsened_potentials,mesh_geometry,new_geometry);
    }

  }



  float new_convergence = 0;
  int iterations = 0;


  auto t1 = std::chrono::high_resolution_clock::now();

  for(iterations = 1; iterations < 10000; iterations++){

    std::vector<std::vector<float>> potentials_copy = potentials; //save for convergence metric


    for(int root_mesh_idx = 0; root_mesh_idx < potentials.size(); root_mesh_idx++){ //iterate over coarse submesh cubes

      int root_idx = root_mesh_idx;
      int root_x;
      int root_y;
      int root_z;
      position_from_index(root_x,root_y,root_z,root_idx,mesh_geometry.root_x_len,mesh_geometry.root_y_len,mesh_geometry.root_z_len);

      for(int submesh_idx = 0; submesh_idx < potentials[root_mesh_idx].size(); submesh_idx++){ //check if submesh is active and iterate over the same
        if(!boundary_conditions[root_mesh_idx][submesh_idx]){

          int x = (root_x*mesh_geometry.sub_len)+(submesh_idx % mesh_geometry.sub_len);
          int y = (root_y*mesh_geometry.sub_len)+((submesh_idx / mesh_geometry.sub_len) % mesh_geometry.sub_len);
          int z = (root_z*mesh_geometry.sub_len)+(submesh_idx / (mesh_geometry.sub_len * mesh_geometry.sub_len)); //this is SO DUMB


          float new_stencil_value = ((get_mesh_value(x+1,y,z,potentials,mesh_geometry) +
                                      get_mesh_value(x-1,y,z,potentials,mesh_geometry) +
                                      get_mesh_value(x,y+1,z,potentials,mesh_geometry) +
                                      get_mesh_value(x,y-1,z,potentials,mesh_geometry) +
                                      get_mesh_value(x,y,z+1,potentials,mesh_geometry) +
                                      get_mesh_value(x,y,z-1,potentials,mesh_geometry))/6.0);

          if(new_stencil_value == 0){ //sparse!
            continue;
          }
          // float current_val = get_mesh_value(x,y,z,potentials,mesh_geometry);
          // float delta = (new_stencil_value-current_val);
          // current_val += delta*overrelaxation;
          // current_val = overrelaxation*current_val-((overrelaxation-1.0)*delta);
          set_mesh_value(new_stencil_value,x,y,z,potentials,mesh_geometry);

        }
      }
    }

    for(int root_mesh_idx = 0; root_mesh_idx < potentials.size(); root_mesh_idx++){ //iterate over coarse submesh cubes
      for(int submesh_idx = 0; submesh_idx < potentials[root_mesh_idx].size(); submesh_idx++){
        if(fabs(potentials[root_mesh_idx][submesh_idx]-potentials_copy[root_mesh_idx][submesh_idx]) > new_convergence){
          new_convergence = fabs(potentials[root_mesh_idx][submesh_idx]-potentials_copy[root_mesh_idx][submesh_idx]); //maximum difference between old and new
        }
      }
    }

    // overrelaxation += new_convergence/max_value;

    printf("convergence: %f\n",new_convergence);

    if(new_convergence < tolerance){ //exit condition
      break;
    }
    new_convergence = 0;

  }

  auto t2 = std::chrono::high_resolution_clock::now();

  std::cout << iterations << " iterations, " << (std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count())/(float) iterations << " ms each" << "\n";
  std::cout << "total time " << (std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()) << " ms" << "\n";

  // for(int i = 0; i < total_mesh_len; i++){ potentials_vector[i] = next_potentials[i];};
  // delete[] potentials;
  // delete[] boundaries;
  // delete[] next_potentials;

  return iterations;
  // return potentials[0];
  //throw std::runtime_error("Laplace did not converge!");
}


// void electric_field(std::vector<float> &potentials, int x, int y, int z, int mesh_geometry[3], float mesh_scale[3], float gradient[3]){
//   /*
//   Basic 3d field gradient computation.pa
//   Returns 0 if position is too close to an edge.
//   */
//   if(x > mesh_geometry[X]-1 || y > mesh_geometry[Y]-1 || y > mesh_geometry[Y]-1){
//     gradient[X] = 0;
//     gradient[Y] = 0;
//     gradient[Z] = 0;
//   }
//   else{
//     gradient[X] = (potentials[i_idx(x+1,y,z,mesh_geometry)]-potentials[i_idx(x,y,z,mesh_geometry)])/mesh_scale[X]; //unsymmetric field measurement; I don't think this matters.
//     gradient[Y] = (potentials[i_idx(x,y+1,z,mesh_geometry)]-potentials[i_idx(x,y,z,mesh_geometry)])/mesh_scale[Y];
//     gradient[Z] = (potentials[i_idx(x,y,z+1,mesh_geometry)]-potentials[i_idx(x,y,z,mesh_geometry)])/mesh_scale[Z];
//   }
// }
//
// void interpolated_electric_field(std::vector<float> &potentials, int x, int y, int z, int mesh_geometry[3], float mesh_scale[3], float gradient[3]){
//
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

int idx_from_position(int x, int y, int z, int x_len, int y_len, int z_len){
  /*
  Helper function to obtain 1D mesh index from int 3D mesh position
  sanity checking should be done in caller for performance reasons
  */
  return (x_len*y_len*z) + (x_len*y) + x;
}

void position_from_index(int &x, int &y, int &z, int index, int x_len, int y_len, int z_len){
  /*
  Helper function to obtain 1D mesh index from int 3D mesh position
  sanity checking should be done in caller for performance reasons
  */
  x = index % x_len; //ugly and slow! Dumb and stupid!
  y = (index / x_len) % y_len; //idx of root cell
  z = index / (x_len * y_len);
}


int i_idx(int x, int y, int z, int mesh_geometry[3]){
  /*
  Helper function to obtain 1D mesh index from int 3D mesh position
  sanity checking should be done in caller for performance reasons
  */
  return 0;
  // return (mesh_geometry[X]*mesh_geometry[Y]*z) + (mesh_geometry.*y) + x ;
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
