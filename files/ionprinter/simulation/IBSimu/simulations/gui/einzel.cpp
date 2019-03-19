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

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
using namespace std;

#ifdef GTK3
#include "gtkplotter.hpp"
#endif

#define BEAM_ENERGY 0.25 //eV

#define ION_CURTAIN_ENERGY 0.1
#define ION_CURTAIN_WIRE_RADIUS 0.001
#define ION_CURTAIN_WIRE_WIDTH 0.0005

#define GRID_SIZE 0.00005 //m

#define MESH_LENGTH 0.004
#define MESH_HEIGHT 0.004

#define MIDPOINT ((MESH_HEIGHT/GRID_SIZE)/2.0)

#define INTERACTIVE_PLOT 0

#define NUMBER_OF_PARTICLES 100
#define DIAGNOSTIC_PARTICLE_INTERPOLATE 30
#define DIAGNOSTIC_X_INTERVAL 5.0*GRID_SIZE

#define PNG_PLOT 1

#define NUMBER_OF_PROCESSES 16.0 //specify domain decomposition

const int MESH_X_SIZE = MESH_LENGTH/GRID_SIZE;
const int MESH_Y_SIZE = MESH_HEIGHT/GRID_SIZE;


double beam_current = 0.0005;
float beam_radius = 0.002;
float beam_x_position = 0.001;
float feature_1_voltage = 200;
float feature_2_voltage = 0;
float feature_3_voltage = 200;

float recombination_point = MESH_LENGTH;

int iteration = 0;
int run_id = 1;

//? ? x,r,voltage

float feature_1_X = 0.0007;
float feature_1_Y = 0.003;
float feature_1_X_len = 0.0011;
float feature_1_Y_len = 0.0003;
float feature_1_gap = 0.0001;


float feature_2_X = feature_1_X+feature_1_X_len+feature_1_gap;
float feature_2_Y = 0.003;
float feature_2_X_len = 0.0011;
float feature_2_Y_len = 0.0003;
float feature_2_gap = 0.0001;


float feature_3_X = feature_2_X+feature_2_X_len+feature_2_gap;
float feature_3_Y = 0.003;
float feature_3_X_len = 0.0011;
float feature_3_Y_len = 0.0003;
float feature_3_gap = 0.0001;



bool feature_1( double x, double y, double z ){
   return((x >= feature_1_X && x <= feature_1_X+feature_1_X_len) && (y >= feature_1_Y && y <= feature_1_Y+feature_1_Y_len));
}

bool feature_2( double x, double y, double z ){
   return((x >= feature_2_X && x <= feature_2_X+feature_2_X_len) && (y >= feature_2_Y && y <= feature_2_Y+feature_2_Y_len));
}

bool feature_3( double x, double y, double z ){
   return((x >= feature_3_X && x <= feature_3_X+feature_3_X_len) && (y >= feature_3_Y && y <= feature_3_Y+feature_3_Y_len));
}

void dump_particles(ParticleDataBaseCyl pdb){ //could be used to determine recombination point?

  //start at zero since particles could go backwards
  stringstream file_prefix;
  file_prefix << "data/1/" << run_id << "/" << "data" << ".csv";
  stringstream log_row;
  log_row.precision(8);


  //later can plot just energy vs min. radius

  //find indices of top and bottom, and save only those?
  // FILE *f = fopen(file_prefix.str().c_str(), "a+b");

  float x_pos = 0;

  for(x_pos = 0; x_pos < MESH_LENGTH-GRID_SIZE; x_pos+=DIAGNOSTIC_X_INTERVAL){
    vector<trajectory_diagnostic_e> diagnostics;
    diagnostics.push_back( DIAG_VR ); //first added is index 0, second is index 1, etc.
    diagnostics.push_back( DIAG_EK );
    diagnostics.push_back( DIAG_R );

    TrajectoryDiagnosticData tdata;
    pdb.trajectories_at_plane( tdata, AXIS_X, x_pos, diagnostics );
    const TrajectoryDiagnosticColumn &diag_radial_position = tdata(2);
    const TrajectoryDiagnosticColumn &diag_energy = tdata(1);
    const TrajectoryDiagnosticColumn &diag_radial_velocity = tdata(0);


    float sum_particle_velocity = 0;
    for( uint32_t i = 0; i < diag_radial_velocity.size(); i+=DIAGNOSTIC_PARTICLE_INTERPOLATE) {
      sum_particle_velocity -= diag_radial_velocity(i);
    }

    float average_particle_energy = 0;
    for( uint32_t i = 0; i < diag_energy.size(); i+=DIAGNOSTIC_PARTICLE_INTERPOLATE) {
      average_particle_energy += diag_energy(i);
    }
    average_particle_energy /= diag_energy.size();

    if(sum_particle_velocity < 0){
      log_row << run_id << ","
              << iteration << ","
              << beam_current << ","
              << beam_radius << ","
              << beam_x_position << ","
              << feature_1_voltage << ","
              << feature_2_voltage << ","
              << feature_3_voltage << "," //block header
              << feature_1_X  << ","
              << feature_1_Y << ","
              << feature_1_X_len << ","
              << feature_1_Y_len << ","
              << feature_1_gap << ","
              << feature_2_X  << ","
              << feature_2_Y << ","
              << feature_2_X_len << ","
              << feature_2_Y_len << ","
              << feature_2_gap << ","
              << feature_3_X  << ","
              << feature_3_Y << ","
              << feature_3_X_len << ","
              << feature_3_Y_len << ","
              << feature_3_gap << ","
              << x_pos << ","
              << sum_particle_velocity << ","
              << average_particle_energy << ","
              << "\n";
      }
  }
  // log_row << "\n\n"; //block terminate
  // fclose(f);
  std::ofstream outfile;
  outfile.open(file_prefix.str(), std::ios_base::app);
  outfile << log_row.str();
  outfile.close();
}

void simu( int *argc, char ** argv )
{

    if(*argc > 1){
      run_id = atoi(argv[1]);
      cout << "Running with ID: " << run_id << "\n";
    }

    Fl_Window *window = new Fl_Window(340,180);
    Fl_Box *box = new Fl_Box(20,40,300,100,"Hello, World!");
    box->box(FL_UP_BOX);
    box->labelfont(FL_BOLD+FL_ITALIC);
    box->labelsize(36);
    box->labeltype(FL_SHADOW_LABEL);
    window->end();
    window->show();
    Fl_Button *button = new Fl_Button(x, y, width, height, "label");

    Fl::run();

    while(1){
      while(Fl::wait()){
        
      }
      clock_t start = clock();

      Geometry geom( MODE_CYL, Int3D(MESH_LENGTH/GRID_SIZE,MESH_HEIGHT/GRID_SIZE,1), Vec3D(0,0,0), GRID_SIZE );

      Solid *s1 = new FuncSolid( feature_1 );
      geom.set_solid( 7, s1 );
      Solid *s2 = new FuncSolid( feature_2 );
      geom.set_solid( 8, s2 );
      Solid *s3 = new FuncSolid( feature_3 );
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
                                              beam_current/beam_area, //beam current density
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

    if(PNG_PLOT){
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
    }

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

    dump_particles(pdb);

    iteration+=1;

    clock_t end = clock();
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("\nEst. simulation time (days): %f\n",((seconds)/NUMBER_OF_PROCESSES));

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
