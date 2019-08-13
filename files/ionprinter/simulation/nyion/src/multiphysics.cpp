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
#include "stencils.hpp"
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
// #include <vtkdoubleArray.h>
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


double scharge_efield(double beam_current, double beam_velocity, double beam_radius, double sample_radius){
    //Calculate the electric field at the edge of a beam
    //Eq. 44 in https://arxiv.org/pdf/1401.3951.pdf
    //returns one value: V/m
    //Beam velocity is in m/s

    return ((beam_current/(2.0*(M_PI)*EPSILON_0*beam_velocity)) * (sample_radius/pow(beam_radius,2.0)));
}


// void v_cycle(std::vector<std::vector<double>> &potentials, std::vector<std::vector<double>> &boundaries, root_mesh_geometry mesh_geometry, double tolerance, int i){
//   /* -----------------------------------------------------------------------------
//   Multigrid v-cycle implementation based off
//   http://www.maths.lth.se/na/courses/FMNN15/media/material/Chapter8.09b__.pdf
//   and Wikipedia's multigrid diagram
//   ----------------------------------------------------------------------------- */
//
//   int stages = 2;
//   int cycle[] = {2,2};
//   double tol[] = {0.5,0.01,0.01};
//
//   /* -----------------------------------------------------------------------------
//   First, iterate once or twice over the finest mesh to obtain residuals.
//   Boundary conditions are required.
//   ----------------------------------------------------------------------------- */
//   std::vector<std::vector<double>> residuals;
//   residuals = gauss_seidel(potentials, boundaries, mesh_geometry, 500, (i==0), (i!=0));
//
//   std::vector<std::vector<double>> rhs;
//
//   coarsen_mesh(residuals,rhs,mesh_geometry,cycle[i]);
//
//   std::vector<std::vector<double>> eps;
//   if(i == stages-1){
//     gauss_seidel(rhs, boundaries, mesh_geometry, tol[i], 0, 1);
//   }
//   else{
//     v_cycle(rhs,boundaries,mesh_geometry,tol[i],i+1);
//   }
//
//
//   std::vector<std::vector<double>> temp;
//   decoarsen_mesh(rhs,temp,mesh_geometry,cycle[i]);
//
//   /* -----------------------------------------------------------------------------
//   Correct original mesh with computed residuals.
//   ----------------------------------------------------------------------------- */
//   for(uint32_t root = 0; root < temp.size(); root++){
//     for(uint32_t sub = 0; sub < temp[root].size(); sub++){
//       potentials[root][sub] += temp[root][sub];
//     }
//   }
//
//   gauss_seidel(potentials, boundaries, mesh_geometry, tolerance, (i==0), (i!=0));
//
// }
