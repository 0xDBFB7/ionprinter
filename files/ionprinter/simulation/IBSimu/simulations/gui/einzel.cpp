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
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Chart.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Value_Output.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Cairo.H>
#include <FL/Fl_Cairo_Window.H>


#include <cairo.h>

#include "plplot/plplot.h"
#include "plplot/plstream.h"
#include "plstream.h"


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
#define DIAGNOSTIC_X_INTERVAL 5.0*GRID_SIZE


#define NUMBER_OF_PROCESSES 16.0 //specify domain decomposition

const int MESH_X_SIZE = MESH_LENGTH/GRID_SIZE;
const int MESH_Y_SIZE = MESH_HEIGHT/GRID_SIZE;

#define BUTTON_HEIGHT 30
#define BUTTON_WIDTH 100

#define WINDOW_X_SIZE 1000
#define WINDOW_Y_SIZE 1000


double beam_current = 0.0005;
float beam_radius = 0.002;
float beam_x_position = 0.001;
float feature_1_voltage = 200;
float feature_2_voltage = 0;
float feature_3_voltage = 200;

float recombination_point = MESH_LENGTH;

int iteration = 0;
int run_id = 1;

//Thanks, https://blog.as.uky.edu/allen/?page_id=677
//and https://groups.google.com/forum/#!msg/fltkgeneral/7ncncZH7qRg/6iIYqmoRBwAJ


void simu( int *argc, char ** argv )
{
    Fl_Window *window = new Fl_Window(WINDOW_X_SIZE,WINDOW_Y_SIZE);
    // Fl_Box *box = new Fl_Box(20,40,300,100,"Hello, World!");
    // box->box(FL_UP_BOX);
    // box->labelfont(FL_BOLD+FL_ITALIC);
    // box->labelsize(36);
    // box->labeltype(FL_SHADOW_LABEL);

    Fl_Chart *particle_chart = new Fl_Chart(10,10,100,100);
    Fl_Value_Slider *beam_current_slider = new Fl_Value_Slider(50,50,100,100);
    Fl_Value_Output *avg_energy = new Fl_Value_Output(50,50,BUTTON_WIDTH,BUTTON_WIDTH,"test");
    avg_energy->value(100.0);
    // Fl_Button *refresh_sim = new Fl_Button(10, 10, BUTTON_WIDTH, BUTTON_HEIGHT, "Refresh Sim");
    Fl_Return_Button *refresh_sim = new Fl_Return_Button(10, 10, BUTTON_WIDTH*2, BUTTON_HEIGHT*2, "@refresh  Refresh Sim");
    Fl_Box *box = new Fl_Box(20,40,300,100,"Hello, World!");
    Fl_Box *beam_plot_box = new Fl_Box(10,10,500,500);
    Fl_Box *test_plot_box = new Fl_Box(0,0,1000,1000);
    // Fl_Group beam_plot_group = new Fl_Group();
    // beam_plot_group->add(beam_plot_box)

    // window->end();
    window->show();


    while(1){

      while(Fl::check()){
        if(refresh_sim->value()){
          break;
        }
      }

      clock_t start = clock();

      Geometry geom( MODE_CYL, Int3D(MESH_LENGTH/GRID_SIZE,MESH_HEIGHT/GRID_SIZE,1), Vec3D(0,0,0), GRID_SIZE );

      // Solid *s1 = new FuncSolid( feature_1 );
      // geom.set_solid( 7, s1 );
      // Solid *s2 = new FuncSolid( feature_2 );
      // geom.set_solid( 8, s2 );
      // Solid *s3 = new FuncSolid( feature_3 );
      // geom.set_solid( 9, s3 );

      geom.set_boundary( 1, Bound(BOUND_NEUMANN,     0.0 ) );
      geom.set_boundary( 2, Bound(BOUND_DIRICHLET,  0.0) );
      geom.set_boundary( 3, Bound(BOUND_NEUMANN,     0.0) );
      geom.set_boundary( 4, Bound(BOUND_NEUMANN,     0.0) );
      // geom.set_boundary( 7, Bound(BOUND_DIRICHLET,  feature_1_voltage) );
      // geom.set_boundary( 8, Bound(BOUND_DIRICHLET,  feature_2_voltage) );
      // geom.set_boundary( 9, Bound(BOUND_DIRICHLET,  feature_3_voltage) );

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


    float x;
    for(float x_pos = 0; x_pos < MESH_LENGTH-GRID_SIZE; x_pos+=DIAGNOSTIC_X_INTERVAL){
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
      for( uint32_t i = 0; i < diag_radial_velocity.size(); i+=1) {
        sum_particle_velocity -= diag_radial_velocity(i);
      }

      float average_particle_energy = 0;
      for( uint32_t i = 0; i < diag_energy.size(); i+=1) {
        average_particle_energy += diag_energy(i);
      }
      average_particle_energy /= diag_energy.size();
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

    // if(PNG_PLOT){
    GeomPlotter geomplotter( geom );
    geomplotter.set_size(750,750);
    geomplotter.set_epot( &epot );
    geomplotter.set_efield( &efield );
    geomplotter.set_bfield( &bfield );
    geomplotter.set_particle_database( &pdb );
    geomplotter.set_fieldgraph_plot(FIELD_BFIELD_Z);
    stringstream file_prefix;
    file_prefix << "beam.png";
    geomplotter.plot_png(file_prefix.str());
    // }

    // // if(INTERACTIVE_PLOT){
    //   GTKPlotter plotter( argc, &argv );
    //   plotter.set_geometry( &geom );
    //   plotter.set_epot( &epot );
    //   plotter.set_efield( &efield );
    //   plotter.set_bfield( &bfield );
    //   plotter.set_scharge( &scharge );
    //   plotter.set_particledatabase( &pdb );
    //   plotter.new_geometry_plot_window();
    //   plotter.run();
    // // }




    iteration+=1;
    clock_t end = clock();
    float sim_duration = (float)(end - start) / CLOCKS_PER_SEC;

    cairo_t         *cairoContext;
    plsetopt( "-geometry", "300x300" );
    plsfnam (	"test.png" );
    plsdev( "pngcairo" );
    plinit();
    pl_cmd( PLESC_DEVINIT, cairoContext );
    plenv( 0.0, 1.0, 0.0, 1.0, 1, 0 );
    pllab("x", "y", "Energy");
    plend();
    // pls->line( NSIZE, x, y );

    // pls->plot3d( x, y, z, XPTS, YPTS, opt[k] | MAG_COLOR, true );

    Fl::flush();
    Fl_PNG_Image beam_plot("beam.png");
    beam_plot_box->image(beam_plot);

    Fl_PNG_Image png("test.png");
    test_plot_box->image(png);
    window->redraw();

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
