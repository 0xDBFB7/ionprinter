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
#include <fstream>

#include <math.h>
using namespace std::chrono;


// #include "tiny_obj_loader.h"
// #include <vtkVersion.h>
// #include <vtkPolyData.h>
// #include <vtkPointData.h>
// #include <vtkCubeSource.h>
// #include <vtkSmartPointer.h>
// #include <vtkSelectEnclosedPoints.h>
// #include <vtkIntArray.h>
// #include <vtkDataArray.h>
// #include <vtkVertexGlyphFilter.h>
// #include <vtkPolyDataMapper.h>
// #include <vtkPolyData.h>
// #include <vtkSTLReader.h>
// #include <vtkSmartPointer.h>
// #include <vtkPolyDataMapper.h>
// #include <vtkSmartPointer.h>
// #include <vtkVoxelModeller.h>
// #include <vtkPolyData.h>
// #include <vtkXMLImageDataWriter.h>
//
// #include <vtkVersion.h>
// #include <vtkSmartPointer.h>
// #include <vtkPoints.h>
// #include <vtkPolyData.h>
// #include <vtkPointData.h>
// #include <vtkCellArray.h>
// #include <vtkUnsignedCharArray.h>
// #include <vtkPolyDataMapper.h>
// #include <vtkActor.h>
// #include <vtkRenderWindow.h>
// #include <vtkRenderer.h>
// #include <vtkRenderWindowInteractor.h>
// #include <vtkVertexGlyphFilter.h>
// #include <vtkProperty.h>
//
// #include <vtkCellArray.h>
// #include <vtkPoints.h>
// #include <vtkXMLPolyDataWriter.h>
// #include <vtkPolyData.h>
// #include <vtkSmartPointer.h>
//
// #include <vtkSmartPointer.h>
// #include <vtkSphereSource.h>
// #include <vtkPolyData.h>
// #include <vtkPointData.h>
// #include <vtkFloatArray.h>
// #include <vtkDoubleArray.h>
// #include <vtkIntArray.h>
//
// #include <mpi.h>
//
//
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
//
// #include <boost/array.hpp>


/* -----------------------------------------------------------------------------
----------------------------------------------------------------------------- */


#define EPSILON_0 8.854187e-12



#define MULTIGRID_COARSE_GRID 0.001
#define MATERIAL_BC_CUTOFF 1000//

float opengl_zoom = 0;
float opengl_z_extent = 0;
float opengl_current_angle_x = 45;
float opengl_current_angle_y = 25.0;
float opengl_current_x_translate = 0.0;
float opengl_current_y_translate = 0.0;
float opengl_current_z_translate = 0;
float opengl_delta_zoom = 0;
float opengl_delta_angle_x = 0;
float opengl_delta_angle_y = 0;
float opengl_delta_x_translate = 0.0;
float opengl_delta_y_translate = 0.0;
float opengl_delta_z_translate = 0.0;
bool opengl_3d_active = 0;
// float mouse_previous_x = 0;
// float mouse_previous_y = 0;

root_mesh_geometry::root_mesh_geometry(float bounds[6], float new_root_scale){
  /*
  Creates a new mesh description with the specified parameters.

  Root scale is in meters per gridpoint.

  Mesh maximum bounds are rounded up to the nearest root_scale.
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
}



double scharge_efield(float beam_current, float beam_velocity, float beam_radius, float sample_radius){
    //Calculate the electric field at the edge of a beam
    //Eq. 44 in https://arxiv.org/pdf/1401.3951.pdf
    //returns one value: V/m
    //Beam velocity is in m/s

    return ((beam_current/(2.0*(M_PI)*EPSILON_0*beam_velocity)) * (sample_radius/pow(beam_radius,2.0)));
}



template<typename T>
void enable_mesh_region(std::vector<std::vector<T>> &input_mesh, float bounds[6], root_mesh_geometry mesh_geometry, int submesh_side_length){
  /* -----------------------------------------------------------------------------
  Resize submeshes within specified bounds to desired submesh_side_length.
  ----------------------------------------------------------------------------- */



  /* -----------------------------------------------------------------------------
  Constrain the newly-activated bounds to the actual bounds of the mesh.
  Round lower bounds down and upper bounds up.
  ----------------------------------------------------------------------------- */
  int x_min = std::max(0,(int)((bounds[X1]-mesh_geometry.x_min_bound)/mesh_geometry.root_scale));
  int x_max = std::min(mesh_geometry.root_x_len,(int) ceil((bounds[X2]-mesh_geometry.x_min_bound)/mesh_geometry.root_scale));
  int y_min = std::max(0,(int)((bounds[Y1]-mesh_geometry.y_min_bound)/mesh_geometry.root_scale));
  int y_max = std::min(mesh_geometry.root_y_len,(int) ceil((bounds[Y2]-mesh_geometry.y_min_bound)/mesh_geometry.root_scale));
  int z_min = std::max(0,(int)((bounds[Z1]-mesh_geometry.z_min_bound)/mesh_geometry.root_scale));
  int z_max = std::min(mesh_geometry.root_z_len,(int) ceil((bounds[Z2]-mesh_geometry.z_min_bound)/mesh_geometry.root_scale));

  input_mesh.resize(mesh_geometry.root_size);

  for(int x = x_min; x < x_max; x++){
    for(int y = y_min; y < y_max; y++){
      for(int z = z_min; z < z_max; z++){
        int index = idx(x,y,z,mesh_geometry.root_x_len,mesh_geometry.root_y_len);
        input_mesh[index].resize(submesh_side_length*submesh_side_length*submesh_side_length);
      }
    }
  }
}
template void enable_mesh_region(std::vector<std::vector<float>> &input_mesh, float bounds[6], root_mesh_geometry mesh_geometry, int submesh_side_length);
template void enable_mesh_region(std::vector<std::vector<int>> &input_mesh, float bounds[6], root_mesh_geometry mesh_geometry, int submesh_side_length);


//sanity-checked getter and setter?

template<typename T>
void coarsen_mesh(std::vector<std::vector<T>> &original, std::vector<std::vector<T>> &coarsened, root_mesh_geometry original_geometry, int scale_divisor){
  /*
  The root mesh resolution remains the same; only the submeshes are coarsened.
  A simple straight-injection operator.
  otherwise known as "restriction" in literature.
  */
  coarsened = original;

  for(int root_mesh_idx = 0; root_mesh_idx < original.size(); root_mesh_idx++){ //iterate over coarse submesh cubes
    if(original[root_mesh_idx].size()){

      int fine_sub_len = submesh_side_length(original[root_mesh_idx]); //cube

      if(fine_sub_len % scale_divisor != 0){
        throw std::invalid_argument("Mesh not evenly divisible by scale factor.");
      }

      int coarse_sub_len = fine_sub_len/scale_divisor;
      coarsened[root_mesh_idx].resize(coarse_sub_len*coarse_sub_len*coarse_sub_len);

      for(int x = 0; x < coarse_sub_len; x++){
        for(int y = 0; y < coarse_sub_len; y++){
          for(int z = 0; z < coarse_sub_len; z++){
            int coarse_sub_idx = idx(x,y,z,coarse_sub_len,coarse_sub_len);
            int fine_sub_idx = idx((x*scale_divisor),(y*scale_divisor),(z*scale_divisor),fine_sub_len,fine_sub_len);

            coarsened[root_mesh_idx][coarse_sub_idx] = original[root_mesh_idx][fine_sub_idx];
          }
        }
      }
    }
  }
}
template void coarsen_mesh(std::vector<std::vector<float>> &original, std::vector<std::vector<float>> &coarsened, root_mesh_geometry original_geometry, int scale_divisor);
template void coarsen_mesh(std::vector<std::vector<int>> &original, std::vector<std::vector<int>> &coarsened, root_mesh_geometry original_geometry, int scale_divisor);


template<typename T>
void decoarsen_mesh(std::vector<std::vector<T>> &original, std::vector<std::vector<T>> &decoarsened, root_mesh_geometry original_geometry, int scale_divisor){
  /*
  The root mesh resolution remains the same; only the submeshes are coarsened.

  otherwise known as "prolongation" in literature.

  Trilinearly interpolates a coarse mesh into a fine one.
  */
  decoarsened = original;

  float interp_value = 0;

  for(int root_mesh_idx = 0; root_mesh_idx < original.size(); root_mesh_idx++){ //iterate over coarse submesh cubes
    if(original[root_mesh_idx].size()){
      int coarse_sub_len = submesh_side_length(original[root_mesh_idx]);
      int fine_sub_len = coarse_sub_len*scale_divisor;

      decoarsened[root_mesh_idx].resize(fine_sub_len*fine_sub_len*fine_sub_len);

      for(int x = 0; x < coarse_sub_len; x++){
        for(int y = 0; y < coarse_sub_len; y++){
          for(int z = 0; z < coarse_sub_len; z++){
            int coarse_sub_idx = (coarse_sub_len*coarse_sub_len*(z)) + (coarse_sub_len*(y)) + (x);

            bool valid = false;

            float x_interp_scale = (relative_mesh_value(original,root_mesh_idx,x,y,z,1,0,0,original_geometry,valid)
                                          -original[root_mesh_idx][coarse_sub_idx])/scale_divisor;
            if(!valid) x_interp_scale = 0; //this will produce incorrect results at the edge of the root mesh.
                                            // extrapolating using previous scale would be a more suitable method.

            float y_interp_scale = (relative_mesh_value(original,root_mesh_idx,x,y,z,0,1,0,original_geometry,valid)
                                          -original[root_mesh_idx][coarse_sub_idx])/scale_divisor;
            if(!valid) y_interp_scale = 0;

            float z_interp_scale = (relative_mesh_value(original,root_mesh_idx,x,y,z,0,0,1,original_geometry,valid)
                                          -original[root_mesh_idx][coarse_sub_idx])/scale_divisor;
            if(!valid) z_interp_scale = 0;

            for(int f_x = 0; f_x < scale_divisor; f_x++){
              for(int f_y = 0; f_y < scale_divisor; f_y++){
                for(int f_z = 0; f_z < scale_divisor; f_z++){
                  int fine_sub_idx = (fine_sub_len*fine_sub_len*((z*scale_divisor)+f_z)) + (fine_sub_len*((y*scale_divisor)+f_y)) + ((x*scale_divisor)+f_x);

                  interp_value = (x_interp_scale*f_x)
                                        + (y_interp_scale*f_y)
                                        + (z_interp_scale*f_z)
                                        + original[root_mesh_idx][coarse_sub_idx];

                  decoarsened[root_mesh_idx][fine_sub_idx] = interp_value;

                }
              }
            }
          }
        }
      }
    }
  }
}
template void decoarsen_mesh(std::vector<std::vector<float>> &original, std::vector<std::vector<float>> &decoarsened, root_mesh_geometry original_geometry, int scale_divisor);
template void decoarsen_mesh(std::vector<std::vector<int>> &original, std::vector<std::vector<int>> &decoarsened, root_mesh_geometry original_geometry, int scale_divisor);


template<typename T>
float relative_mesh_value(std::vector<std::vector<T>>& input_mesh, int root, int s_x, int s_y, int s_z, int x_rel, int y_rel, int z_rel, root_mesh_geometry mesh_geometry, bool &valid){
  /* -----------------------------------------------------------------------------
  s_x,y,z refer to cell index within submesh.

  Since the submesh side lengths are variable, one can't index directly into the array.

  Currently only indexes into the 8 directly adjacent points.

  int * valid is a pseudo-return value that indicates whether the requested position is defined.
  It's useful to return a value of 0.0 if we run off the edge of the mesh; this is a simple way of defining
  the required edge-of-domain ghost points for infinite boundary potential solvers.
  However, all other solvers must ignore undefined points; hence the valid flag.
  ----------------------------------------------------------------------------- */


  int this_submesh_side_length = submesh_side_length(input_mesh[root]);

  /* -----------------------------------------------------------------------------
  Determine which root submesh contains the new point. Index in a relative manner.
  ----------------------------------------------------------------------------- */
  int r_x_rel = 0;
  int r_y_rel = 0;
  int r_z_rel = 0;

  if(x_rel == -1 && s_x == 0){ r_x_rel = -1;};
  if(x_rel == 1 && s_x == this_submesh_side_length-1){ r_x_rel = 1;};
  if(y_rel == -1 && s_y == 0){ r_y_rel = -1;};
  if(y_rel == 1 && s_y == this_submesh_side_length-1){ r_y_rel = 1;};
  if(z_rel == -1 && s_z == 0){ r_z_rel = -1;};
  if(z_rel == 1 && s_z == this_submesh_side_length-1){ r_z_rel = 1;};

  int root_i = root + idx(r_x_rel,r_y_rel,r_z_rel,mesh_geometry.root_x_len,mesh_geometry.root_y_len);

  /* -----------------------------------------------------------------------------
  Make sure that's a valid submesh
  ----------------------------------------------------------------------------- */
  if(root_i >= 0 && root_i < mesh_geometry.root_size && input_mesh[root_i].size()){

    int new_submesh_side_length = submesh_side_length(input_mesh[root_i]);

    /* -----------------------------------------------------------------------------
    Correct for differing current and adjacent submesh side lengths
    ----------------------------------------------------------------------------- */
    float scale_factor = new_submesh_side_length / ((float) this_submesh_side_length);

    int new_sub_x = 0;
    int new_sub_y = 0;
    int new_sub_z = 0;

    /* -----------------------------------------------------------------------------
    If we've skipped to the next submesh, adjust the submesh index accordingly
    to get the adjacent cell. Otherwise, add the relative move to the submesh index.
    ----------------------------------------------------------------------------- */
    if(r_x_rel == -1) new_sub_x = new_submesh_side_length-1;
    else if(r_x_rel == 1) new_sub_x = 0;
    else new_sub_x = (int)((s_x+x_rel)*scale_factor);

    if(r_y_rel == -1) new_sub_y = new_submesh_side_length-1;
    else if(r_y_rel == 1) new_sub_y = 0;
    else new_sub_y = (int)((s_y+y_rel)*scale_factor);

    if(r_z_rel == -1) new_sub_z = new_submesh_side_length-1;
    else if(r_z_rel == 1) new_sub_z = 0;
    else new_sub_z = (int)((s_z+z_rel)*scale_factor);

    valid = true;
    return input_mesh[root_i][idx(new_sub_x,new_sub_y,new_sub_z,new_submesh_side_length,new_submesh_side_length)];
  }
  else{
    valid = false;
    return 0.0;
  }
}
template float relative_mesh_value(std::vector<std::vector<float>>& input_mesh, int root, int s_x, int s_y, int s_z, int x_rel, int y_rel, int z_rel, root_mesh_geometry mesh_geometry, bool &valid);
template float relative_mesh_value(std::vector<std::vector<int>>& input_mesh, int root, int s_x, int s_y, int s_z, int x_rel, int y_rel, int z_rel, root_mesh_geometry mesh_geometry, bool &valid);



template<typename T>
int submesh_side_length(std::vector<T> &input_vector){
  /*

  */
  return round(std::cbrt(input_vector.size()));
}
template int submesh_side_length(std::vector<float> &input_vector);
template int submesh_side_length(std::vector<int> &input_vector);




void v_cycle(std::vector<std::vector<float>> &potentials, std::vector<std::vector<int>> &boundaries, root_mesh_geometry mesh_geometry, float tolerance, int i){
  /* -----------------------------------------------------------------------------
  Multigrid v-cycle implementation based off
  http://www.maths.lth.se/na/courses/FMNN15/media/material/Chapter8.09b__.pdf
  and Wikipedia's multigrid diagram
  ----------------------------------------------------------------------------- */

  int stages = 2;
  int cycle[] = {2,2};
  float tol[] = {0.5,0.01,0.01};

  /* -----------------------------------------------------------------------------
  First, iterate once or twice over the finest mesh to obtain residuals.
  Boundary conditions are required.
  ----------------------------------------------------------------------------- */
  std::vector<std::vector<float>> residuals;
  residuals = gauss_seidel(potentials, boundaries, mesh_geometry, 500, (i==0), (i!=0));

  std::vector<std::vector<float>> rhs;

  coarsen_mesh(residuals,rhs,mesh_geometry,cycle[i]);

  std::vector<std::vector<float>> eps;
  if(i == stages-1){
    gauss_seidel(rhs, boundaries, mesh_geometry, tol[i], 0, 1);
  }
  else{
    v_cycle(rhs,boundaries,mesh_geometry,tol[i],i+1);
  }


  std::vector<std::vector<float>> temp;
  decoarsen_mesh(rhs,temp,mesh_geometry,cycle[i]);

  /* -----------------------------------------------------------------------------
  Correct original mesh with computed residuals.
  ----------------------------------------------------------------------------- */
  for(int root = 0; root < temp.size(); root++){
    for(int sub = 0; sub < temp[root].size(); sub++){
      potentials[root][sub] += temp[root][sub];
    }
  }

  gauss_seidel(potentials, boundaries, mesh_geometry, 50, (i==0), (i!=0));

}




std::vector<std::vector<float>> gauss_seidel(std::vector<std::vector<float>> &potentials, std::vector<std::vector<int>> &boundaries,
                                                                                                root_mesh_geometry mesh_geometry, float tolerance, bool field, bool ignore_boundaries){
  /*
  Jacobi averages the four nearest points and stores the result in a new matrix.
  Gauss-Seidel averages the four nearest points and updates the current matrix.
  SOR/SSOR is essentially Gauss-Seidel * 1 to 2.0.
  Conjugate Gradient. Converges in something like sqrt() that of Gauss-Seidel.
  Alternative stencils (8-point etc) can also improve convergence, but are computationally expensive.
  */


  // float standard_normal_scalar = 1000.0; //some algorithms converge faster if values are near 1; therefore, we scale the input.

  int root_x = mesh_geometry.root_x_len;
  int root_y = mesh_geometry.root_y_len;
  int root_z = mesh_geometry.root_z_len;
  int root_size = (root_x*root_y*root_z);
  int root_xy = (root_x * root_y);

  std::vector<std::vector<float>> potentials_copy = potentials;


  auto t1 = std::chrono::high_resolution_clock::now();

  int iterations = 0;

  float new_convergence = 0;

  for(iterations = 1; iterations < 10000; iterations++){

    potentials_copy = potentials;

    for(int root = 0; root < root_size; root++){
      if(potentials[root].size()){

        int this_submesh_side_length = submesh_side_length(potentials[root]); //slow AF
        int this_submesh_side_length_squared = this_submesh_side_length*this_submesh_side_length;

        std::vector<float> this_potential_submesh = potentials[root];

        for(int x = 0; x < this_submesh_side_length; x++){ //first do the edges, including the boundary check
          for(int y = 0; y < this_submesh_side_length; y++){ // this could be made even more efficient by doing a side at a time
            for(int z = 0; z < this_submesh_side_length; z++){

              int sub_idx = (this_submesh_side_length_squared*z) + (this_submesh_side_length*y) + x;

              if(!boundaries[root][sub_idx] || ignore_boundaries){

                //for a typical 60^3 submesh, this part of the loop will only run 0.0166x as many points as the rest
                if(x == 0 || y == 0 || z == 0 || x == this_submesh_side_length-1 //if we're at an edge or corner
                                              || y == this_submesh_side_length-1
                                              || z == this_submesh_side_length-1){ //not quite as fast as 6 seperate loops, but that's tedious and boring


                  /*
                  The 'field' and 'stencil_divisor' flags serve to differentiate between in-conductor current simulations, or
                  vacuum field sims.

                  If this a field sim, no boundary condition cells should be updated; their value is set externally.
                  Additionally, all edges of the simulation must be forced to zero.
                  This is effected by always dividing by 6.

                  However, we're using the BC array to also denote conductor presence; if the
                  */

                  int stencil_divisor = 6;
                  float stencil_value = 0;
                  bool valid = false;

                  stencil_value += relative_mesh_value(potentials,root,x,y,z,0,0,1,mesh_geometry,valid);
                  if(!valid) stencil_divisor--;
                  stencil_value += relative_mesh_value(potentials,root,x,y,z,0,0,-1,mesh_geometry,valid);
                  if(!valid) stencil_divisor--;
                  stencil_value += relative_mesh_value(potentials,root,x,y,z,0,1,0,mesh_geometry,valid);
                  if(!valid) stencil_divisor--;
                  stencil_value += relative_mesh_value(potentials,root,x,y,z,0,-1,0,mesh_geometry,valid);
                  if(!valid) stencil_divisor--;
                  stencil_value += relative_mesh_value(potentials,root,x,y,z,1,0,0,mesh_geometry,valid);
                  if(!valid) stencil_divisor--;
                  stencil_value += relative_mesh_value(potentials,root,x,y,z,-1,0,0,mesh_geometry,valid);
                  if(!valid) stencil_divisor--;

                  if(!field){
                    stencil_value /= stencil_divisor;
                  }
                  else{
                    stencil_value /= 6.0;
                  }
                  potentials[root][sub_idx] = stencil_value;
                }
                else{

                  potentials[root][sub_idx] =       (this_potential_submesh[sub_idx-1] +
                                                     this_potential_submesh[sub_idx+1] +
                                                     this_potential_submesh[sub_idx-this_submesh_side_length] +
                                                     this_potential_submesh[sub_idx+this_submesh_side_length] +
                                                     this_potential_submesh[sub_idx-this_submesh_side_length_squared] +
                                                     this_potential_submesh[sub_idx+this_submesh_side_length_squared])/6.0; //this will be changed for !field sim

                }
              }
            }
          }
        }
      }
    }

    // if(iterations % 20 == 0){

      for(int root = 0; root < root_size; root++){ //re-add boundaries - saves the additional check
        if(potentials[root].size()){
          for(int sub = 0; sub < potentials[root].size(); sub++){
            if(fabs(potentials[root][sub]-potentials_copy[root][sub]) > new_convergence){
              new_convergence = fabs(potentials[root][sub]-potentials_copy[root][sub]); //maximum difference between old and new
            }
          }
        }
      }

      printf("convergence: %f\n",new_convergence);
      if((new_convergence) < tolerance){ //exit condition
        break;
      }
    // }
    new_convergence = 0;

  }

  auto t2 = std::chrono::high_resolution_clock::now();

  std::vector<std::vector<float>> residuals = potentials;
  for(int root = 0; root < potentials.size(); root++){
    for(int sub = 0; sub < potentials[root].size(); sub++){
      residuals[root][sub] = (potentials[root][sub] - potentials_copy[root][sub]);
    }
  }


  std::cout << iterations << " iterations, " << (std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count())/(float) iterations << " ms each" << "\n";
  std::cout << "total time " << (std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()) << " ms" << "\n";

  return residuals;
}




void to_csv(std::vector<std::vector<float>> &original, root_mesh_geometry mesh_geometry){
  std::ofstream output_file;
  output_file.open ("out.csv");

  for(int r_x = 0; r_x < mesh_geometry.root_x_len; r_x++){
    for(int r_y = 0; r_y < mesh_geometry.root_y_len; r_y++){
      for(int r_z = 0; r_z < mesh_geometry.root_z_len; r_z++){
        int root_idx = (mesh_geometry.root_x_len*mesh_geometry.root_y_len*r_z) + (mesh_geometry.root_x_len*r_y) + r_x;

        if(original[root_idx].size()){
          int fine_sub_len = submesh_side_length(original[root_idx]);
          for(int x = 0; x < fine_sub_len; x++){
            for(int y = 0; y < fine_sub_len; y++){
              for(int z = 0; z < fine_sub_len; z++){
                int sub_idx = (fine_sub_len*fine_sub_len*z) + (fine_sub_len*y) + x;
                float world_x = (r_x*mesh_geometry.root_scale)+(x*(mesh_geometry.root_scale/fine_sub_len));
                float world_y = (r_y*mesh_geometry.root_scale)+(y*(mesh_geometry.root_scale/fine_sub_len));
                float world_z = (r_z*mesh_geometry.root_scale)+(z*(mesh_geometry.root_scale/fine_sub_len));
                // if(original[root_idx][sub_idx]){
                  output_file << world_x << ","
                              << world_y << ","
                              << world_z << ","
                              << original[root_idx][sub_idx] << "," << "\n";
                // }
              }
            }
          }
        }
      }
    }
  }
  output_file.close();
}






//
// void mouse_handler(int x, int y){
//   opengl_angle_x = ((x-mouse_previous_x)/(float)OPENGL_WINDOW_X)*360.0;
//   opengl_angle_y = ((y-mouse_previous_y)/(float)OPENGL_WINDOW_Y)*360.0;
//   mouse_previous_x = x;
//   mouse_previous_y = y;
//
// }

void keyboard_handler(unsigned char key, int x, int y){
  if(key == '4') opengl_delta_angle_x = 10;
  if(key == '6') opengl_delta_angle_x = -10;
  if(key == '8') opengl_delta_angle_y = 10;
  if(key == '2') opengl_delta_angle_y = -10;
  if(key == '+') opengl_delta_zoom = -10;
  if(key == '-') opengl_delta_zoom = +10;
  if(key == '0'){
      opengl_delta_angle_x -= opengl_current_angle_x;
      opengl_delta_angle_y -= opengl_current_angle_y;
  }
}

void initialize_opengl(root_mesh_geometry mesh_geometry){
  int argc = 0;
  char *argv[1];
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glEnable(GL_MULTISAMPLE);
  glutInitWindowSize(OPENGL_WINDOW_X, OPENGL_WINDOW_Y);
  glutCreateWindow("Nyion");
  glViewport(0, 0, OPENGL_WINDOW_X, OPENGL_WINDOW_Y);

  glutKeyboardFunc(keyboard_handler);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black and opaque
  glClearDepth(1.0f);                   // Set background depth to farthest
  glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
  glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
  glShadeModel(GL_SMOOTH);   // Enable smooth shading
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

  glEnable (GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* -----------------------------------------------------------------------------
  Determine the longest side of the mesh and configure the frustrum appropriately.
  ----------------------------------------------------------------------------- */
  // int min_extent = std::min(std::min(mesh_geometry.x_min_bound,mesh_geometry.y_min_bound),mesh_geometry.z_min_bound)/OPENGL_WORLD_SCALE;
  // int max_extent = std::max(std::max(mesh_geometry.x_max_bound,mesh_geometry.y_max_bound),mesh_geometry.z_max_bound)/OPENGL_WORLD_SCALE;
  // opengl_z_extent = max_extent-min_extent;
  //
  // opengl_current_z_translate = 2*opengl_z_extent;
  // opengl_current_x_translate = -(((mesh_geometry.x_max_bound-mesh_geometry.x_min_bound)/2)+mesh_geometry.x_min_bound)/OPENGL_WORLD_SCALE;
  // opengl_current_y_translate = -(((mesh_geometry.y_max_bound-mesh_geometry.y_min_bound)/2)+mesh_geometry.y_min_bound)/OPENGL_WORLD_SCALE;

  opengl_3d_active = false;
  opengl_3d_mode();

  // glutPassiveMotionFunc(mouse_handler);
  // glutMouseFunc(void (*func)(int button, int state,
  //                                 int x, int y));


  /* -----------------------------------------------------------------------------
  Set viewport parameters
  ----------------------------------------------------------------------------- */

  // glTranslatef(opengl_current_x_translate,opengl_current_y_translate, -opengl_current_z_translate);

  glPushMatrix();
    glTranslatef(0,0,0);
    glColor3f(255,255,255);
    glutSolidCube(3);
  glPopMatrix();

  opengl_draw_axis_cross();
}

void opengl_3d_mode(){
  if (!opengl_3d_active) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    double aspect = (double)viewport[2] / (double)viewport[3];
    printf("%f,%f\n",opengl_current_z_translate-2*opengl_z_extent,opengl_current_z_translate+opengl_z_extent);
    gluPerspective(60, aspect, opengl_current_z_translate-2*opengl_z_extent, opengl_current_z_translate+opengl_z_extent); //all parameters must be positive.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    opengl_3d_active = 1;
  }
}


void opengl_2d_mode(){
  if (opengl_3d_active) {

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, OPENGL_WINDOW_X, OPENGL_WINDOW_Y, 0.0, -1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // glDisable(GL_CULL_FACE);
    //
    // glClear(GL_DEPTH_BUFFER_BIT);

    opengl_3d_active = 0;
  }
}


void opengl_clear_screen(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPopMatrix();
}


void opengl_draw_axis_cross(){
  /* -----------------------------------------------------------------------------
  Draws a pretty RGB cross denoting X,Y,Z.
  ----------------------------------------------------------------------------- */

  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_LINE_STRIP);
  glVertex3f(300,0,0);
  glVertex3f(0,0,0);
  glEnd();

  glColor3f(0.0f, 1.0f, 0.0f);
  glBegin(GL_LINE_STRIP);
  glVertex3f(0,300,0);
  glVertex3f(0,0,0);
  glEnd();

  glColor3f(0.0f, 0.0f, 1.0f);
  glBegin(GL_LINE_STRIP);
  glVertex3f(0,0,300);
  glVertex3f(0,0,0);
  glEnd();
}


void draw_geometry_outline(root_mesh_geometry mesh_geometry){
  /* -----------------------------------------------------------------------------
  Draw a wireframe box encompassing mesh_geometry.
  GL_LINE_STRIPs should be drawn between pixels; hence the +0.5.
  ----------------------------------------------------------------------------- */
  glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    glVertex3f(((int)(mesh_geometry.x_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_min_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_min_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_max_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_max_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_min_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_min_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_min_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_max_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_max_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_min_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_min_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_min_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_max_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_max_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_max_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_max_bound/OPENGL_WORLD_SCALE)+0.5));
    glVertex3f(((int)(mesh_geometry.x_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.y_min_bound/OPENGL_WORLD_SCALE)+0.5),
                          ((int)(mesh_geometry.z_max_bound/OPENGL_WORLD_SCALE)+0.5));
  glEnd();

}

template<typename T>
void draw_mesh(std::vector<std::vector<T>>& input_mesh ,root_mesh_geometry mesh_geometry){
  /* -----------------------------------------------------------------------------
  Determine min and max for color scaling
  ----------------------------------------------------------------------------- */
  float min = 1e10;
  float max = 1e-10;
  for(int root = 0; root < mesh_geometry.root_size; root++){ //re-add boundaries - saves the additional check
    if(input_mesh[root].size()){
      for(int sub = 0; sub < input_mesh[root].size(); sub++){
        if(input_mesh[root][sub] < min){
          min = input_mesh[root][sub];
        }
        if(input_mesh[root][sub] > max){
          max = input_mesh[root][sub];
        }
      }
    }
  }
  /* -----------------------------------------------------------------------------
  Draw cubes
  ----------------------------------------------------------------------------- */
  for(int r_x = 0; r_x < mesh_geometry.root_x_len; r_x++){
    for(int r_y = 0; r_y < mesh_geometry.root_y_len; r_y++){
      for(int r_z = 0; r_z < mesh_geometry.root_z_len; r_z++){
        int root_idx = (mesh_geometry.root_x_len*mesh_geometry.root_y_len*r_z) + (mesh_geometry.root_x_len*r_y) + r_x;

        if(input_mesh[root_idx].size()){
          /* -----------------------------------------------------------------------------
          Draw submesh outline
          ----------------------------------------------------------------------------- */
          glPushMatrix();
            int cube_size = mesh_geometry.root_scale/OPENGL_WORLD_SCALE;
            glTranslatef((r_x*mesh_geometry.root_scale)/OPENGL_WORLD_SCALE+(cube_size/2.0),
                          (r_y*mesh_geometry.root_scale)/OPENGL_WORLD_SCALE+(cube_size/2.0),
                          (r_z*mesh_geometry.root_scale)/OPENGL_WORLD_SCALE+(cube_size/2.0));
            glColor4f(1, 0, 0, 1);
            glutWireCube(cube_size);
          glPopMatrix();
          /* -----------------------------------------------------------------------------
          Draw fine mesh cubes
          ----------------------------------------------------------------------------- */
          int sub_len = submesh_side_length(input_mesh[root_idx]);

          cube_size = (mesh_geometry.root_scale/sub_len)/OPENGL_WORLD_SCALE;
          float fine_scale = mesh_geometry.root_scale/sub_len;
          for(int x = 0; x < sub_len; x++){
            for(int y = 0; y < sub_len; y++){
              for(int z = 0; z < sub_len; z++){
                glPushMatrix();

                  glTranslatef((r_x*mesh_geometry.root_scale+x*fine_scale)/OPENGL_WORLD_SCALE+(cube_size/2.0),
                                (r_y*mesh_geometry.root_scale+y*fine_scale)/OPENGL_WORLD_SCALE+(cube_size/2.0),
                                (r_z*mesh_geometry.root_scale+z*fine_scale)/OPENGL_WORLD_SCALE+(cube_size/2.0));
                  glColor3f(255,255,255);
                  glutWireCube((mesh_geometry.root_scale/sub_len)/OPENGL_WORLD_SCALE);
                glPopMatrix();
              }
            }
          }

        }
        else{
          /* -----------------------------------------------------------------------------
          Draw inactive submesh outlines
          ----------------------------------------------------------------------------- */
          glPushMatrix();
            int cube_size = mesh_geometry.root_scale/OPENGL_WORLD_SCALE;
            glTranslatef((r_x*mesh_geometry.root_scale)/OPENGL_WORLD_SCALE+(cube_size/2.0),
                          (r_y*mesh_geometry.root_scale)/OPENGL_WORLD_SCALE+(cube_size/2.0),
                          (r_z*mesh_geometry.root_scale)/OPENGL_WORLD_SCALE+(cube_size/2.0));
            // glColor3f(255,255,255);
            glColor4f(1, 1, 1, 0.02);
            glutSolidCube(cube_size);
          glPopMatrix();
        }
      }
    }
  }
}
template void draw_mesh(std::vector<std::vector<float>>& input_mesh,root_mesh_geometry mesh_geometry);
template void draw_mesh(std::vector<std::vector<int>>& input_mesh,root_mesh_geometry mesh_geometry);


void opengl_apply_camera_rotation(){
  /* -----------------------------------------------------------------------------
  Undo model rotation and translation
  ----------------------------------------------------------------------------- */
  glRotatef(-opengl_current_angle_y, 0, 1.0, 0);
  glRotatef(-opengl_current_angle_x, 1.0, 0, 0);
  glTranslatef(-opengl_current_x_translate, -opengl_current_y_translate, -opengl_current_z_translate);
  // opengl_current_x_translate += opengl_delta_zoom;
  // opengl_current_y_translate += opengl_delta_zoom;
  opengl_current_z_translate += opengl_delta_zoom;

  opengl_current_angle_x += opengl_delta_angle_x;
  opengl_current_angle_y += opengl_delta_angle_y;
  /* -----------------------------------------------------------------------------
  Redo model rotation and translation
  ----------------------------------------------------------------------------- */
  glTranslatef(opengl_current_x_translate, opengl_current_y_translate, opengl_current_z_translate);
  glRotatef(opengl_current_angle_x, 1.0, 0, 0);
  glRotatef(opengl_current_angle_y, 0, 1.0, 0);
  opengl_delta_zoom = 0;
  opengl_delta_angle_x = 0;
  opengl_delta_angle_y = 0;
}

void update_screen(){
  glutMainLoopEvent();
  // opengl_3d_mode();
  // opengl_draw_axis_cross();

  // opengl_apply_camera_rotation();

  glutSwapBuffers();
  glutPostRedisplay();
  // TakeScreenshot(t);
}



// void TakeScreenshot(float time)
// {
// 	unsigned char *buffer;
// 	char filename[200];
// 	int w = 1920;
// 	int h = 1080;
// 	int buf_size = 18 + (w * h * 3);
// 	int i;
// 	unsigned char temp;
// 	FILE *out_file;
// 	char filename2[200];
// 	sprintf(filename2,"video/%i.tga",shot_counter);
// 	if (!(out_file = fopen(filename2, "wb")))
// 	{
// 		return;
// 	}
//
// 	// allocate mem to read from frame buf
// 	if (!(buffer = (unsigned char *) calloc(1, buf_size)))
// 	{
// 		return;
// 	}
//
// 	// set header info
// 	buffer[2] = 2;	// uncompressed
// 	buffer[12] = w & 255;
// 	buffer[13] = w >> 8;
// 	buffer[14] = h & 255;
// 	buffer[15] = h >> 8;
// 	buffer[16] = 24;	// 24 bits per pix
//
// 	// read frame buf
// 	glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, buffer + 18);
//
// 	// RGB to BGR
// 	for (i = 18; i < buf_size; i += 3)
// 	{
// 		temp = buffer[i];
// 		buffer[i] = buffer[i + 2];
// 		buffer[i + 2] = temp;
// 	}
// 	// write header + color buf to file
// 	fwrite(buffer, sizeof(unsigned char), buf_size, out_file);
// 	// cleanup
// 	fclose(out_file);
// 	free(buffer);
// 	shot_counter++;
// }



// void import_mesh(const char* filename, std::vector<bool> &mesh_present, int mesh_geometry[3], float mesh_scale[3], double bounds[6]){
//   /*
//   Deposit a mesh onto a uniform grid.
//   Inputs are filename c_string, a 1-D mesh with length
//
//   The convoluted mesh_geometry setup is used because 3d vectors have large overhead in C++
//   mesh_geometry stores the dimensions of the mesh in indices, mesh scale stores grid to world scale in meters
//
//   Outputs bounds of imported mesh.
//   */
//   vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
//   reader->SetFileName(filename);
//   reader->Update();
//
//   vtkSmartPointer<vtkVoxelModeller> voxelModeller = vtkSmartPointer<vtkVoxelModeller>::New();
//   reader->GetOutput()->GetBounds(bounds);
//   for(int i = 0; i < 6; i++) bounds[i] /= 1000.0; //convert mm to m
//
//   // for(int i = 0; i < 6; i++){bounds[i] += translate[i/2];}; //translate
//
//   vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
//   vtkSmartPointer<vtkPolyData> pointsPolydata = vtkSmartPointer<vtkPolyData>::New();
//   //Points inside test
//   vtkSmartPointer<vtkSelectEnclosedPoints> selectEnclosedPoints =
//     vtkSmartPointer<vtkSelectEnclosedPoints>::New();
//   selectEnclosedPoints->SetInputData(pointsPolydata);
//   selectEnclosedPoints->SetSurfaceConnection(reader->GetOutputPort());
//
//   double point[3] = {0, 0.0, 0.0};
//   points->InsertNextPoint(point);
//
//   // throw std::invalid_argument( "received negative value" );
//
//
//   for(int x = std::max(bounds[0]/mesh_scale[X],0.0); x < std::min((int)(bounds[1]/mesh_scale[X]),mesh_geometry[X]); x++){
//     for(int y = std::max(bounds[2]/mesh_scale[Y],0.0); y < std::min((int)(bounds[3]/mesh_scale[Y]),mesh_geometry[Y]); y++){
//       for(int z = std::max(bounds[4]/mesh_scale[Z],0.0); z < std::min((int)(bounds[5]/mesh_scale[Z]),mesh_geometry[Z]); z++){
//         point[X] = (x*mesh_scale[X]);
//         point[Y] = (y*mesh_scale[Y]);
//         point[Z] = (z*mesh_scale[Z]);
//         points->SetPoint(0,point);
//         pointsPolydata->SetPoints(points);
//         selectEnclosedPoints->Update();
//         mesh_present[i_idx(x,y,z,mesh_geometry)] = selectEnclosedPoints->IsInside(0);
//       }
//     }
//   }
// }
