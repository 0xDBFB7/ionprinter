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

#include <Magick++.h>

#include "perlin.h"

using namespace std;

#ifdef GTK3
#include "gtkplotter.hpp"
#endif

#define BEAM_CURRENT 0.0024251 //A 35
#define BEAM_ENERGY 0.25 //eV

#define ION_CURTAIN_ENERGY 0.1
#define ION_CURTAIN_WIRE_RADIUS 0.001
#define ION_CURTAIN_WIRE_WIDTH 0.0005

#define GRID_SIZE 0.00005 //m

#define MESH_LENGTH 0.004
#define MESH_HEIGHT 0.004

#define MIDPOINT ((MESH_HEIGHT/GRID_SIZE)/2.0)

#define INTERACTIVE_PLOT 0

//evolution parameters
#define BASE_VOLTAGE 100
#define VOLTAGE_DEVIATION 50.0


#define SEED_EVOLVE_THRESHOLD 0.05

#define MAX_ITERATIONS 10000000000

#define NUMBER_OF_PARTICLES 1000


const int MESH_X_SIZE = MESH_LENGTH/GRID_SIZE;
const int MESH_Y_SIZE = MESH_HEIGHT/GRID_SIZE;

float beam_radius = 0.002;

float recombination_point = 0;

int iteration = 0;

string run_id = "test";

// there's no reason why we can't run many, many copies of this in parallel - no disk writes are required
// gnu parallel

float feature_1_grid[MESH_X_SIZE][MESH_Y_SIZE];
float feature_2_grid[MESH_X_SIZE][MESH_Y_SIZE];
float feature_3_grid[MESH_X_SIZE][MESH_Y_SIZE];

float beam_x_position = 0;

float feature_1_voltage = 0;
float feature_2_voltage = 0;
float feature_3_voltage = 0;

float current_fitness = 0.0;


float random_float(float bound){
  return ((float)rand()/(float)(RAND_MAX))*bound;
}


std::tuple <float, float> lowest_radial_v_pos(ParticleDataBaseCyl pdb){ //could be used to determine recombination point?
  float lowest_position = 0;
  float previous_lowest = 0;

  for(float x_pos = beam_x_position; x_pos < (MESH_LENGTH/GRID_SIZE)-1; x_pos++){
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

void dump_arrays(){
  stringstream file_prefix;
  file_prefix << "data/1/" << run_id << "/" << "feature_1_grid" << iteration << ".bin";
  FILE *f = fopen(file_prefix.str().c_str(), "wb");
  fwrite(feature_1_grid, sizeof(float), sizeof(feature_1_grid), f);
  fclose(f);

  file_prefix.str("");
  file_prefix << "data/1/" << run_id << "/" << "feature_2_grid" << iteration << ".bin";
  f = fopen(file_prefix.str().c_str(), "wb");
  fwrite(feature_1_grid, sizeof(float), sizeof(feature_1_grid), f);
  fclose(f);

  file_prefix.str("");
  file_prefix << "data/1/" << run_id << "/" << "feature_3_grid" << iteration << ".bin";
  f = fopen(file_prefix.str().c_str(), "wb");
  fwrite(feature_1_grid, sizeof(float), sizeof(feature_1_grid), f);
  fclose(f);
}


float surviving_particle_count(ParticleDataBaseCyl pdb){ //could be used to determine recombination point?
  float lowest_position = 0;
  float previous_lowest = 0;

  vector<trajectory_diagnostic_e> diagnostics;
  diagnostics.push_back( DIAG_VR );
  TrajectoryDiagnosticData tdata;
  pdb.trajectories_at_plane( tdata, AXIS_X, MESH_LENGTH-GRID_SIZE, diagnostics ); //just before the end
  const TrajectoryDiagnosticColumn &rad_v = tdata(0);
  return rad_v.size();
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

void create_meshes(){
  Image image;
}


// void fill_perlin(){
//     for(int x = 0; x < MESH_X_SIZE; x++){
//       for(int y = 0; y < MESH_Y_SIZE; y++){
//         feature_1_grid[x][y] = perlin2d(x, y, 0.05, 4, iteration);
//       }
//     }
//     for(int x = 0; x < MESH_X_SIZE; x++){
//       for(int y = 0; y < MESH_Y_SIZE; y++){
//         feature_2_grid[x][y] = perlin2d(x, y, 0.05, 4, iteration+1);
//       }
//     }
//     for(int x = 0; x < MESH_X_SIZE; x++){
//       for(int y = 0; y < MESH_Y_SIZE; y++){
//         feature_3_grid[x][y] = perlin2d(x, y, 0.05, 4, iteration+2);
//       }
//     }
// }
//
// void filter_perlin(){
//     for(int x = 0; x < MESH_X_SIZE; x++){
//       for(int y = 0; y < MESH_Y_SIZE; y++){
//         if(feature_1_grid[x][y] > 0.7){
//           feature_1_grid[x][y] = 1.0;
//         }
//       }
//     }
//     for(int x = 0; x < MESH_X_SIZE; x++){
//       for(int y = 0; y < MESH_Y_SIZE; y++){
//         if(feature_2_grid[x][y] > 0.7){
//           feature_2_grid[x][y] = 1.0;
//         }
//       }
//     }
//     for(int x = 0; x < MESH_X_SIZE; x++){
//       for(int y = 0; y < MESH_Y_SIZE; y++){
//         if(feature_3_grid[x][y] > 0.7){
//           feature_3_grid[x][y] = 1.0;
//         }
//       }
//     }
// }

void seed_algorithm(){
  feature_1_voltage = BASE_VOLTAGE+(-VOLTAGE_DEVIATION/2.0+random_float(VOLTAGE_DEVIATION));
  feature_2_voltage = BASE_VOLTAGE+(-VOLTAGE_DEVIATION/2.0+random_float(VOLTAGE_DEVIATION));
  feature_3_voltage = 0;
  printf("Feature 1 voltage: %f\n",feature_1_voltage);
  printf("Feature 2 voltage: %f\n",feature_2_voltage);
  printf("Feature 3 voltage: %f\n",feature_3_voltage);

  // fill_perlin();
  // filter_perlin();

}



#define FITNESS_SURVIVING_COUNT_GAIN -50
#define FITNESS_LOWEST_RADIAL_V_GAIN 10

#define FIT_SAVE_THRESHOLD

float fitness(ParticleDataBaseCyl pdb){


  int low_rad_v_x, radial_v_at_pos;
  tie(low_rad_v_x, radial_v_at_pos) = lowest_radial_v_pos(pdb);

  float final_max_e = final_beam_energy(pdb);

  float surviving_particle_ratio = surviving_particle_count(pdb)/(float)NUMBER_OF_PARTICLES;

  float final_fitness = (-1.0*radial_v_at_pos);



  stringstream log_row;
  log_row.precision(4);

  log_row << iteration << "," << surviving_particle_ratio << "," << final_max_e << "," << radial_v_at_pos << ","
            << final_fitness << "," << feature_1_voltage << "," << feature_2_voltage << "," << feature_3_voltage << "\n";

  cout << "\n###############################################################\n";
  cout << log_row.str();
  cout << "\n###############################################################\n";


  std::ofstream outfile;
  stringstream file_prefix;
  file_prefix << "data/1/" << run_id << "/" << "log" << ".csv";
  outfile.open(file_prefix.str(), std::ios_base::app);
  outfile << log_row.str();
  outfile.close();
}

bool einzel_1( double x, double y, double z )
{
  int x_1 = x/GRID_SIZE;
  int y_1 = y/GRID_SIZE;

  return feature_1_grid[x_1][y_1] == 1.0;
}


bool einzel_2( double x, double y, double z )
{
  int x_1 = x/GRID_SIZE;
  int y_1 = y/GRID_SIZE;

  return feature_2_grid[x_1][y_1] == 1.0;
}
//
bool einzel_3( double x, double y, double z )
{
  int x_1 = x/GRID_SIZE;
  int y_1 = y/GRID_SIZE;

  return feature_3_grid[x_1][y_1] == 1.0;
}



void simu( int *argc, char ** argv )
{

    Magick::InitializeMagick(*argv);


    if(*argc > 1){
      run_id = argv[1];
      cout<<"Running with ID: " << run_id << "\n";
    }

    while(iteration < MAX_ITERATIONS){
      srand(time(NULL));   // Initialization, should only be called once.
      seed_algorithm();
      Geometry geom( MODE_CYL, Int3D(MESH_LENGTH/GRID_SIZE,MESH_HEIGHT/GRID_SIZE,1), Vec3D(0,0,0), GRID_SIZE );


      Solid *s1 = new FuncSolid( einzel_1 );
      geom.set_solid( 7, s1 );
      Solid *s2 = new FuncSolid( einzel_2 );
      geom.set_solid( 8, s2 );
      Solid *s3 = new FuncSolid( einzel_3 );
      geom.set_solid( 9, s3 );

      geom.set_boundary( 1, Bound(BOUND_NEUMANN,     0.0 ) );
      geom.set_boundary( 2, Bound(BOUND_DIRICHLET,  0.0) );
      geom.set_boundary( 3, Bound(BOUND_NEUMANN,     0.0) );
      geom.set_boundary( 4, Bound(BOUND_NEUMANN,     0.0) );
      geom.set_boundary( 7, Bound(BOUND_DIRICHLET,  feature_1_voltage) );
      geom.set_boundary( 8, Bound(BOUND_DIRICHLET,  feature_2_voltage) );
      geom.set_boundary( 9, Bound(BOUND_DIRICHLET,  feature_3_voltage) );

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

        float beam_area = (M_PI*pow(beam_radius,2));
        printf("Beam_area: %f\n",beam_area);
      	pdb.add_2d_beam_with_energy(
                                              NUMBER_OF_PARTICLES, //number of particles
                                              BEAM_CURRENT/beam_area, //beam current density
                                              1.0, //charge per particle
                                              29, //amu
                                              BEAM_ENERGY, //eV
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


    // vector<trajectory_diagnostic_e> diagnostics;
    // diagnostics.push_back( DIAG_EK );
    // diagnostics.push_back( DIAG_X );
    // TrajectoryDiagnosticData tdata;
    // pdb.trajectories_at_plane( tdata, AXIS_X, 0.0001, diagnostics );
    // const TrajectoryDiagnosticColumn &energy = tdata(1);
    // const TrajectoryDiagnosticColumn &energy = tdata(0);
    //
    // for( uint32_t i = 0; i < energy.size(); i++ ) {
    //   of_trajdiag << energy(i) << " "
    // }
    //
    // Histogram1D energy_histo( 20, energy.data(), current.data() );
    // vector<double> energy_data = energy_histo.get_data();
    // for( uint32_t i = 0; i < energy_histo.n(); i++ ) {
    //    energy_histo.coord(i) << " " << energy_histo(i) << "\n";
    // }
    //
    // of_histo.close();
    // double range[2];
    // energy_histo.get_range( range );
    //
    // GeomPlotter geomplotter( geom );
    // geomplotter.set_size(1000,1000);
    // geomplotter.set_epot( &epot );
    // geomplotter.set_efield( &efield );
    // geomplotter.set_bfield( &bfield );
    // geomplotter.set_particle_database( &pdb );
    // geomplotter.set_fieldgraph_plot(FIELD_BFIELD_Z);
    // std::stringstream fmt;
    // fmt << "images/" << iteration << ".png";
    // geomplotter.plot_png(fmt.str());


    GeomPlotter geomplotter( geom );
    geomplotter.set_size(1000,1000);
    geomplotter.set_epot( &epot );
    geomplotter.set_efield( &efield );
    geomplotter.set_bfield( &bfield );
    geomplotter.set_particle_database( &pdb );
    geomplotter.set_fieldgraph_plot(FIELD_BFIELD_Z);
    stringstream file_prefix;
    file_prefix << "data/1/" << run_id << "/" << iteration << ".png";
    geomplotter.plot_png(file_prefix.str());

    if(INTERACTIVE_PLOT){
      GTKPlotter plotter( argc, &argv );
      plotter.set_geometry( &geom );
      plotter.set_epot( &epot );
      plotter.set_efield( &efield );
      plotter.set_bfield( &bfield );
      plotter.set_scharge( &scharge );
      plotter.set_particledatabase( &pdb );
      plotter.new_geometry_plot_window();
      plotter.run();
    }

    fitness(pdb);
    dump_arrays();

    iteration+=1;
  }
}


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
