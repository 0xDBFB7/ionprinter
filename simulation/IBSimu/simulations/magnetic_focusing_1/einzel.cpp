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

#include <cstdlib>
#include <ctime>
#include <iostream>


using namespace std;

#ifdef GTK3
#include "gtkplotter.hpp"
#endif

#define BEAM_RADIUS 0.0005
#define BEAM_IR 0

#define BEAM_CURRENT 0.0005 //A 35
#define BEAM_ENERGY 0.2 //eV

#define ION_CURTAIN_ENERGY 10
#define ION_CURTAIN_WIRE_RADIUS 0.001
#define ION_CURTAIN_WIRE_WIDTH 0.0005

#define GRID_SIZE 0.00003 //m

#define MESH_LENGTH 0.03
#define MESH_WIDTH 0.006

#define MESH_X_SIZE MESH_LENGTH/GRID_SIZE
#define MESH_Y_SIZE MESH_WIDTH/GRID_SIZE

#define MIDPOINT ((MESH_WIDTH/GRID_SIZE)/2.0)

#define INTERACTIVE_PLOT 1


float beam_radius = 0.0005;
float beam_y_offset = 0.0;

float recombination_point = 0.1;

// there's no reason why we can't run many, many copies of this in parallel - no disk writes are required
// gnu parallel

// feature_1_grid;
// feature_2_grid;
// feature_3_grid;
//

int iteration = 0;

#define RECOMBINATION_POINT 0.05
// bool einzel_1( double x, double y, double z )
// {
//   return(x < ACCEL_ELECTRODE_X && (y >= ACCEL_ELECTRODE_HOLE_RADIUS));
// }



#define EINZEL_1_X 0.002
#define EINZEL_1_THICKNESS 0.0011
#define EINZEL_1_HEIGHT 0.0002
#define EINZEL_1_Y 0.0025

#define EINZEL_GAP 0.0005

#define EINZEL_2_X EINZEL_1_X+EINZEL_GAP
#define EINZEL_2_THICKNESS 0.001
#define EINZEL_2_HEIGHT 0.0002
#define EINZEL_2_Y 0.0025

#define EINZEL_3_X EINZEL_2_X+EINZEL_2_THICKNESS+EINZEL_GAP
#define EINZEL_3_THICKNESS 0.001
#define EINZEL_3_HEIGHT 0.0002
#define EINZEL_3_Y 0.0025

bool accelerate( double x, double y, double z )
{
  //return(x < 0.001 && (y >= 0.0115 || y <= 0.0095));
   return((x >= 0.0004 && x <= 0.0004+0.00015) && (y >= 0.002 && y <= 0.008));
  // return((x >= EINZEL_1_X-EINZEL_1_THICKNESS && x <= EINZEL_1_X) && (y >= EINZEL_1_Y && y <= EINZEL_1_Y+EINZEL_1_HEIGHT));
  // return ((x >= 0.001 && x <= 0.0013) && )
}

// bool accelerate2( double x, double y, double z )
// {
//   //return(x < 0.001 && (y >= 0.0115 || y <= 0.0095));
//    return((x >= 0.002 && x <= 0.0023) && (y >= 0.003 && y <= 0.008));
//   // return((x >= EINZEL_1_X-EINZEL_1_THICKNESS && x <= EINZEL_1_X) && (y >= EINZEL_1_Y && y <= EINZEL_1_Y+EINZEL_1_HEIGHT));
//   // return ((x >= 0.001 && x <= 0.0013) && )
// }
//
// bool einzel_1( double x, double y, double z )
// {
//   //return(x < 0.001 && (y >= 0.0115 || y <= 0.0095));
//    return((x >= EINZEL_1_X-EINZEL_1_THICKNESS && x <= EINZEL_1_X) && (y >= EINZEL_1_Y && y <= EINZEL_1_Y+EINZEL_1_HEIGHT));
//   // return((x >= EINZEL_1_X-EINZEL_1_THICKNESS && x <= EINZEL_1_X) && (y >= EINZEL_1_Y && y <= EINZEL_1_Y+EINZEL_1_HEIGHT));
//   // return ((x >= 0.001 && x <= 0.0013) && )
// }
//
// bool einzel_2( double x, double y, double z )
// {
//   return((x >= EINZEL_2_X && x <= EINZEL_2_X+EINZEL_2_THICKNESS) && (y >= EINZEL_2_Y && y <= EINZEL_2_Y+EINZEL_2_HEIGHT));
// }
// //
// bool einzel_3( double x, double y, double z )
// {
//   //return(x < 0.001 && (y >= 0.0115 || y <= 0.0095));
//   return((x >= EINZEL_3_X && x <= EINZEL_3_X+EINZEL_3_THICKNESS) && (y >= EINZEL_3_Y && y <= EINZEL_3_Y+EINZEL_3_HEIGHT));
// }


std::tuple <float, float> lowest_radial_v_pos(ParticleDataBaseCyl pdb){ //could be used to determine recombination point?
  float lowest_position = 0;
  float previous_lowest = 0;

  for(float x_pos = 0.001; x_pos < (MESH_LENGTH/GRID_SIZE)-1; x_pos++){
    vector<trajectory_diagnostic_e> diagnostics;
    diagnostics.push_back( DIAG_VR );
    TrajectoryDiagnosticData tdata;
    pdb.trajectories_at_plane( tdata, AXIS_X, x_pos*GRID_SIZE, diagnostics );
    const TrajectoryDiagnosticColumn &rad_v = tdata(0);

    float particle_average = 0;
    for( uint32_t i = 0; i < rad_v.size(); i++ ) {
      particle_average += rad_v(i); //totally rad brotha!
    }
    particle_average /= rad_v.size();
    if(!isnan(particle_average) && (particle_average < previous_lowest || previous_lowest == 0)){
      previous_lowest = particle_average;
      lowest_position = x_pos*GRID_SIZE;
    }
  }
  return {lowest_position,previous_lowest};
  //returns point where velocity is most inward and said inward velocity
}

float final_beam_energy(ParticleDataBaseCyl pdb){ //could be used to determine recombination point?
  float max_e = 0;

  vector<trajectory_diagnostic_e> diagnostics;
  diagnostics.push_back( DIAG_EK );
  TrajectoryDiagnosticData tdata;
  pdb.trajectories_at_plane( tdata, AXIS_X, MESH_LENGTH-GRID_SIZE, diagnostics );
  const TrajectoryDiagnosticColumn &energy = tdata(0);

  for( uint32_t i = 0; i < energy.size(); i++ ) {
    if(energy(i) > max_e){
      max_e = energy(i);
    }
  }
  return max_e;
}


void simu( int *argc, char ***argv )
{
    while(iteration < 1){

      Geometry geom( MODE_CYL, Int3D(MESH_LENGTH/GRID_SIZE,MESH_WIDTH/GRID_SIZE,1), Vec3D(0,0,0), GRID_SIZE );
      //
      // Solid *s1 = new FuncSolid( accelerate );
      // geom.set_solid( 7, s1 );
      //
      // Solid *s2 = new FuncSolid( accelerate2 );
      // geom.set_solid( 8, s2 );
      // 
      // Solid *s1 = new FuncSolid( einzel_1 );
      // geom.set_solid( 7, s1 );
      // Solid *s2 = new FuncSolid( einzel_2 );
      // geom.set_solid( 8, s2 );
      // Solid *s3 = new FuncSolid( einzel_3 );
      // geom.set_solid( 9, s3 );

      geom.set_boundary( 1, Bound(BOUND_NEUMANN,     0.0 ) );
      geom.set_boundary( 2, Bound(BOUND_DIRICHLET,  0.0) );
      geom.set_boundary( 3, Bound(BOUND_NEUMANN,     0.0) );
      // geom.set_boundary( 4, Bound(BOUND_NEUMANN,     0000.0) );
      geom.set_boundary( 7, Bound(BOUND_DIRICHLET,  1000.0) );
      geom.set_boundary( 8, Bound(BOUND_DIRICHLET,  1000.0) );
      geom.set_boundary( 9, Bound(BOUND_DIRICHLET,  000.0) );

      geom.build_mesh();

      EpotUMFPACKSolver solver( geom );
      //EpotBiCGSTABSolver solver( geom );

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

        float beam_area = (M_PI*pow(BEAM_IR+BEAM_RADIUS,2))-(M_PI*pow(BEAM_IR,2));
        printf("Beam_area: %f",beam_area);

        pdb.add_2d_beam_with_energy(
                                              1000, //number of particles
                                              0.0005/beam_area, //beam current density
                                              1.0, //charge per particle
                                              26, //amu 26
                                              BEAM_ENERGY, //eV
                                              0.1,//Normal temperature
                                              0.1,
                                              0.0001,BEAM_IR, //point 1
                                              0.0001,BEAM_IR+BEAM_RADIUS //point 2
                                              );
      	// pdb.add_2d_beam_with_energy(
        //                                       1000, //number of particles
        //                                       5/beam_area, //beam current density
        //                                       -1.0, //charge per particle
        //                                       0.000548, //amu 26
        //                                       100, //eV
        //                                       0.0,//Normal temperature
        //                                       0.0,
        //                                       0.002,BEAM_IR+BEAM_RADIUS, //point 1
        //                                       0.002,BEAM_IR //point 2
        //                                       );



        float ion_curtain_area = 2*M_PI*ION_CURTAIN_WIRE_RADIUS*ION_CURTAIN_WIRE_WIDTH;
        //
        // pdb.add_2d_beam_with_energy(
        //                                       1000, //number of particles
        //                                       10/beam_area, //beam current density
        //                                       -1.0, //charge per particle
        //                                       0.000548, //amu
        //                                       ION_CURTAIN_ENERGY, //eV
        //                                       0.3,//Normal temperature
        //                                       0.1,
        //                                       0.0015+ION_CURTAIN_WIRE_WIDTH,ION_CURTAIN_WIRE_RADIUS, //point 1
        //                                       0.0015,ION_CURTAIN_WIRE_RADIUS //point 2
        //                                       );

      	pdb.iterate_trajectories( scharge, efield, bfield );

        //Make a histogram of particle energies
        //https://sourceforge.net/p/ibsimu/mailman/message/28374280/



        for( int ii = 0; ii < epot.size(0); ii++ ) {
          for( int jj = 0; jj < epot.size(1); jj++ ) {
            for( int kk = 0; kk < epot.size(2); kk++ ) {
              // double z = scharge.h()*kk+scharge.origo(2);
               if(ii > RECOMBINATION_POINT/GRID_SIZE) {
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
               if(ii > RECOMBINATION_POINT/GRID_SIZE) {
                scharge(ii,jj,kk) = 0;
               }
            }
          }
        }
      }


    vector<trajectory_diagnostic_e> diagnostics;
    diagnostics.push_back( DIAG_CURR );
    diagnostics.push_back( DIAG_EK );
    TrajectoryDiagnosticData tdata;
    pdb.trajectories_at_plane( tdata, AXIS_X, 0.002, diagnostics );
    const TrajectoryDiagnosticColumn &energy = tdata(1);
    const TrajectoryDiagnosticColumn &current = tdata(0);

    ofstream of_trajdiag( "trajdiag.dat" );
    for( uint32_t i = 0; i < energy.size(); i++ ) {
      of_trajdiag << energy(i) << " "
            << current(i) << "\n";
    }
    of_trajdiag.close();

    Histogram1D energy_histo( 20, energy.data(), current.data() );
    vector<double> energy_data = energy_histo.get_data();
    ofstream of_histo( "histo.dat" );
    for( uint32_t i = 0; i < energy_histo.n(); i++ ) {
       of_histo << energy_histo.coord(i) << " " << energy_histo(i) << "\n";
    }
    of_histo.close();
    double range[2];
    energy_histo.get_range( range );
    std::cout << "range[0] = " << range[0] << "\n";
    std::cout << "range[1] = " << range[1] << "\n";
    if( range[0] == range[1] )
      std::cout << "ranges equal\n";


    GeomPlotter geomplotter( geom );
    geomplotter.set_size( 1000, 1000 );
    geomplotter.set_epot( &epot );
    geomplotter.set_efield( &efield );
    geomplotter.set_bfield( &bfield );
    geomplotter.set_particle_database( &pdb );
    geomplotter.set_fieldgraph_plot(FIELD_BFIELD_Z);
    std::stringstream fmt;
    fmt << "images/" << iteration << ".png";
    geomplotter.plot_png(fmt.str());

    // printf("Final beam energy: %f",final_beam_energy(pdb));
    //
    // int low_rad_v_x, radial_v_at_pos;
    // tie(low_rad_v_x, radial_v_at_pos) = lowest_radial_v_pos(pdb);
    //
    // printf("Optimal recombination point: %f, velocity: ", low_rad_v_x);

    if(INTERACTIVE_PLOT){
      GTKPlotter plotter( argc, argv );
      plotter.set_geometry( &geom );
      plotter.set_epot( &epot );
      plotter.set_efield( &efield );
      plotter.set_bfield( &bfield );
      plotter.set_scharge( &scharge );
      plotter.set_particledatabase( &pdb );
      plotter.new_geometry_plot_window();
      plotter.run();
    }


    iteration+=1;

  }
}


int main( int argc, char **argv )
{
    try {
        ibsimu.set_message_threshold( MSG_VERBOSE, 1 );
	ibsimu.set_thread_count( 10 );
        simu( &argc, &argv );
    } catch ( Error e ) {
	e.print_error_message( ibsimu.message( 0 ) );
        exit( 1 );
    }

    return( 0 );
}
