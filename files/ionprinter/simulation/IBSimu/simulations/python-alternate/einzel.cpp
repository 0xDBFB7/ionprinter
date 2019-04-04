#include "epot_bicgstabsolver.hpp"
#include "epot_umfpacksolver.hpp"
#include "particledatabase.hpp"
#include "geometry.hpp"
#include "func_solid.hpp"
#include "epot_efield.hpp"
#include "meshvectorfield.hpp"
#include "ibsimu.hpp"
#include "error.hpp"
#include "particlediagplotter.hpp"
#include "geomplotter.hpp"
#include "config.h"
#include <tuple>

#include <cstdlib>
#include <ctime>
#include <iostream>

#include <time.h>
#include <stdlib.h>

using namespace std;

#ifdef GTK3
#include "gtkplotter.hpp"
#endif


#define ION_CURTAIN_ENERGY 0.1
#define ION_CURTAIN_WIRE_RADIUS 0.001
#define ION_CURTAIN_WIRE_WIDTH 0.0005

#define GRID_SIZE 0.00005 //m

#define MESH_X 0.004
#define MESH_Y 0.004

#define IBSIMU_PLOT_RESOLUTION 125000

#define MIDPOINT ((MESH_Y/GRID_SIZE)/2.0)

#define INTERACTIVE_PLOT 0

#define NUMBER_OF_PARTICLES 500
#define DIAGNOSTIC_X_INTERVAL (5.0*GRID_SIZE)


#define NUMBER_OF_PROCESSES 16.0 //specify domain decomposition

const int MESH_X_SIZE = (MESH_X/GRID_SIZE);
const int MESH_Y_SIZE = (MESH_Y/GRID_SIZE);

const int X_DIAGNOSTIC_COUNT = ((MESH_X-GRID_SIZE)/(DIAGNOSTIC_X_INTERVAL));


#define BUTTON_Y 30
#define BUTTON_WIDTH 100

#define WINDOW_X_SIZE 1500
#define WINDOW_Y_SIZE 1000

#define PL_PNG_OFFSET -200

double beam_input_energy = 0.25; //eV
double beam_current = 0.0005;
float beam_radius = 0.002;
float beam_x_position = 0.001;

float recombination_point = MESH_X;


int iteration = 0;
int run_id = 1;


void simu( int *argc, char ** argv )
{

      Geometry geom( MODE_CYL, Int3D(MESH_X/GRID_SIZE,MESH_Y/GRID_SIZE,1), Vec3D(0,0,0), GRID_SIZE );

      geom.set_boundary( 1, Bound(BOUND_NEUMANN,     0.0 ) );
      geom.set_boundary( 2, Bound(BOUND_DIRICHLET,  0.0) );
      geom.set_boundary( 3, Bound(BOUND_NEUMANN,     0.0) );
      geom.set_boundary( 4, Bound(BOUND_NEUMANN,     0.0) );



      geom.build_mesh();

      EpotUMFPACKSolver solver( geom );

      EpotField epot( geom );
      MeshScalarField scharge( geom );
      EpotEfield efield( epot );

      field_extrpl_e efldextrpl[6] = { FIELD_EXTRAPOLATE, FIELD_EXTRAPOLATE,
        FIELD_SYMMETRIC_POTENTIAL, FIELD_EXTRAPOLATE,
        FIELD_EXTRAPOLATE, FIELD_EXTRAPOLATE };
      efield.set_extrapolation( efldextrpl );

      ParticleDataBaseCyl pdb( geom );
      pdb.set_thread_count(10);
      bool pmirror[6] = { false, false, false, false, false, false };
      pdb.set_mirror( pmirror );

      bool fout[3] = { true, true, true };
      MeshVectorField bfield( geom, fout);


      for( size_t i = 0; i < 15; i++ ) {
      	solver.solve( epot, scharge );
      	efield.recalculate();
      	pdb.clear();

        //float beam_area = BEAM_RADIUS*2; //m
        //see https://sourceforge.net/p/ibsimu/mailman/message/31283552/

        float beam_area = (M_PI*pow(beam_radius,2));
        printf("Beam_area: %f\n",beam_area);
      	pdb.add_2d_beam_with_energy(
                                              NUMBER_OF_PARTICLES, //number of particles
                                              beam_current/beam_area, //beam current density
                                              1.0, //charge per particle
                                              29, //amu
                                              beam_input_energy, //eV
                                              0.2,//Normal temperature
                                              0.1,
                                              beam_x_position,0, //point 1
                                              beam_x_position,0+beam_radius //point 2
                                              );

        float ion_curtain_area = 2*M_PI*ION_CURTAIN_WIRE_RADIUS*ION_CURTAIN_WIRE_WIDTH;

        // pdb.add_2d_beam_with_energy(
        //                                       1000, //number of particles
        //                                       BEAM_CURRENT/beam_area, //beam current density
        //                                       -1.0, //charge per particle
        //                                       0.000548, //amu
        //                                       ION_CURTAIN_ENERGY, //eV
        //                                       0.3,//Normal temperature
        //                                       0.1,
        //                                       0.0015+ION_CURTAIN_WIRE_WIDTH,ION_CURTAIN_WIRE_RADIUS, //point 1
        //                                       0.0015,ION_CURTAIN_WIRE_RADIUS //point 2
        //                                       );
        //
      	pdb.iterate_trajectories( scharge, efield, bfield );

        //Make a histogram of particle energies
        //https://sourceforge.net/p/ibsimu/mailman/message/28374280/



        for( int ii = 0; ii < epot.size(0); ii++ ) {
          for( int jj = 0; jj < epot.size(1); jj++ ) {
            for( int kk = 0; kk < epot.size(2); kk++ ) {
              // double z = scharge.h()*kk+scharge.origo(2);
               if(ii > recombination_point/GRID_SIZE) {
                 epot(ii,jj,kk) = 0;
               }
            }
          }
        }

        //Rather than adjust the charge of the particles after recombination,
        //I'm nulling out the space charge field.
        //'s easier, ya see.
        //code block taken from the IBSimu mailing list.
        for( int ii = 0; ii < scharge.size(0); ii++ ) {
          for( int jj = 0; jj < scharge.size(1); jj++ ) {
            for( int kk = 0; kk < scharge.size(2); kk++ ) {
              // double z = scharge.h()*kk+scharge.origo(2);
               if(ii > recombination_point/GRID_SIZE) {
                scharge(ii,jj,kk) = 0;
               }
            }
          }
        }
      }
  }
}
// from examples/fltk_example.cpp

int main( int argc, char ** argv )
{

    try {
        ibsimu.set_message_threshold( MSG_VERBOSE, 1 );
	ibsimu.set_thread_count( 10 );
        simu( &argc, argv );
    } catch ( Error e ) {
	e.print_error_message( ibsimu.message( 0 ) );
        exit( 1 );
    }

    return( 0 );
}
