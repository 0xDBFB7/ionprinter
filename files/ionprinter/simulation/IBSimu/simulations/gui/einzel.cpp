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
#include <dxf_solid.hpp>
#include <mydxfinsert.hpp>
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
#include <FL/Fl_Simple_Counter.H>

#include <unistd.h>

#include <algorithm>    // std::min_element, std::max_element


#include <mgl2/mgl.h>
#include "mgl2/fltk.h"

#include <cairo.h>

#include "plplot/plplot.h"
#include "plplot/plstream.h"
#include "plstream.h"

#include <Magick++.h>
#include <iostream>

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

#define WINDOW_X_SIZE 700
#define WINDOW_Y_SIZE 700

#define PL_PNG_OFFSET -200

double beam_input_energy = 0.25; //eV
double beam_current = 0.0005;
float beam_radius = 0.002;
float beam_x_position = 0.001;
float feature_1_voltage = 200;
float feature_2_voltage = 0;
float feature_3_voltage = 200;

float recombination_point = MESH_X;


int iteration = 0;
int run_id = 1;

// int current_solid_number = 0;
// std::vector<Solid> feature_solids; //oh no! it's global!
// std::vector<float> feature_voltages;
// std::vector<float> feature_points;

//Thanks, https://blog.as.uky.edu/allen/?page_id=677
//and https://groups.google.com/forum/#!msg/fltkgeneral/7ncncZH7qRg/6iIYqmoRBwAJ

int current_solid = 0;
int currently_selected_solid = 0;
int number_of_features = 1;
float feature_voltages[100];
Magick::Image image;
Magick::PixelPacket *pixels;

int image_w;
int image_h;
int image_range;

// int solids_counter = 0;
// bool is_in_solid( double x, double y, double z )
// {
//   int row = (y/GRID_SIZE);
//   int column = (x/GRID_SIZE);
//   Magick::Color color = pixels[image_w * row + column];
//   // printf("color: %f\n", (color.redQuantum() / image_range));
//   // return((x >= EINZEL_3_X && x <= EINZEL_3_X+EINZEL_3_THICKNESS) && (y >= EINZEL_3_Y && y <= EINZEL_3_Y+EINZEL_3_Y));
//   // if((color.redQuantum())){
//   //   printf("%i,%i\n",color.redQuantum()/256,current_solid);
//   // }
//   return ((int)(color.redQuantum()/256));
// }

Fl_PNG_Image* beam_envelope;
Fl_PNG_Image* IBSimu_plot;

PLFLT    **particle_x_velocities, **particle_y_velocities;
plstream *pls;

double particle_x_coords[X_DIAGNOSTIC_COUNT];
double particle_y_coords[NUMBER_OF_PARTICLES];


// void feature_select_callback(Fl_Widget *w) {
//   feature_voltages[i];
// }

void simu( int *argc, char ** argv )
{



    //////////////////////////graphics initialization///////////////////////////
    Fl_Window *window = new Fl_Window(WINDOW_X_SIZE,WINDOW_Y_SIZE);
    // Fl_Window *plot_window = new Fl_Window(WINDOW_X_SIZE,WINDOW_Y_SIZE);

    // Fl_Box *box = new Fl_Box(20,40,300,100,"Hello, World!");
    // box->box(FL_UP_BOX);
    // box->labelfont(FL_BOLD+FL_ITALIC);
    // box->labelsize(36);
    // box->labeltype(FL_SHADOW_LABEL);

    // Fl_Chart *particle_chart = new Fl_Chart(10,10,100,100);
    Fl_Value_Slider *feature_voltage_slider = new Fl_Value_Slider(30,30,60,400, "Selected feature \nvoltage");
    feature_voltage_slider->bounds(-5000.0,5000.0);
    Fl_Value_Slider *beam_radius_slider = new Fl_Value_Slider(400,30,60,400, "Beam radius (m)");
    beam_radius_slider->bounds(0.0001,0.004);
    beam_radius_slider->step(0.0001);
    Fl_Simple_Counter *feature_counter = new Fl_Simple_Counter(200,30,100,60, "Selected feature");
    feature_counter->bounds(0,100.0);
    feature_counter->step(1);
    Fl_Simple_Counter *feature_number = new Fl_Simple_Counter(100,30,100,60, "Total features");
    feature_number->bounds(0,100.0);
    feature_number->step(1);
    // Fl_Float_Input *beam_current_input = new Fl_Float_Input(100,600,100,100);
    Fl_Counter *beam_current_input = new Fl_Counter(WINDOW_X_SIZE-BUTTON_WIDTH*4,WINDOW_Y_SIZE-BUTTON_Y*2-20,
                                                                                BUTTON_WIDTH*2,BUTTON_Y*2, "Heavy beam current (amps)");
    beam_current_input->step(0.0001,0.001);
    // Fl_Value_Output *avg_energy = new Fl_Value_Output(50,50,BUTTON_WIDTH,BUTTON_WIDTH,"test");
    // avg_energy->value(100.0);

    // Fl_Button *refresh_sim = new Fl_Button(10, 10, BUTTON_WIDTH, BUTTON_Y, "Refresh Sim");
    Fl_Return_Button *refresh_sim = new Fl_Return_Button(WINDOW_X_SIZE-BUTTON_WIDTH*2, WINDOW_Y_SIZE-BUTTON_Y*2-20,
                                              BUTTON_WIDTH*2, BUTTON_Y*2, "@refresh  Refresh Sim");
    // Fl_Box *box = new Fl_Box(20,40,300,100,"Hello, World!");
    // Fl_Box *IBSimu_plot_box = new Fl_Box(10,0,500,500);
    // Fl_Box *beam_envelope_box = new Fl_Box(500,0,1000,1000);
    // Fl_Group beam_plot_group = new Fl_Group();
    // beam_plot_group->add(beam_plot_box)

    window->show();

    while(1){

      while(Fl::check()){
        feature_voltages[currently_selected_solid] = feature_voltage_slider->value()+1;
        number_of_features = feature_number->value();
        if(refresh_sim->value()){
          break;
        }

        beam_current = beam_current_input->value();
        if(feature_counter->value() != currently_selected_solid){
          currently_selected_solid = feature_counter->value();
          feature_voltage_slider->scrollvalue(feature_voltages[currently_selected_solid],10,-5000.0,10000);
        }
      }

      // Magick::InitializeMagick(*argv);
      // image.read("solid.png");
      // image_w = image.columns();
      // image_h = image.rows();
      // image_range = pow(2, image.modulusDepth());
      // pixels = image.getPixels(0, 0, image_w, image_h);


      clock_t start = clock();

      Geometry geom( MODE_CYL, Int3D(MESH_X/GRID_SIZE,MESH_Y/GRID_SIZE,1), Vec3D(0,0,0), GRID_SIZE );


      geom.set_boundary( 1, Bound(BOUND_NEUMANN,     0.0 ) );
      geom.set_boundary( 2, Bound(BOUND_DIRICHLET,  0.0) );
      geom.set_boundary( 3, Bound(BOUND_NEUMANN,     0.0) );
      geom.set_boundary( 4, Bound(BOUND_NEUMANN,     0.0) );

      MyDXFFile *dxffile = new MyDXFFile( "solid.dxf" );
      for(int i = 0; i < number_of_features; i++) {
        stringstream solid_file_prefix;
        solid_file_prefix << i+1;
        DXFSolid *s1 = new DXFSolid( dxffile,solid_file_prefix.str());
        // s1->define_2x3_mapping( DXFSolid::rotz );
        geom.set_solid(i+7, s1 );
        geom.set_boundary(i+7, Bound(BOUND_DIRICHLET,  feature_voltages[i]) );
      }
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
                                              beam_input_energy, //eV
                                              0.1,//Normal temperature
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
    geomplotter.set_size(500,500);
    geomplotter.set_epot( &epot );
    geomplotter.set_efield( &efield );
    geomplotter.set_bfield( &bfield );
    geomplotter.set_particle_database( &pdb );
    geomplotter.set_fieldgraph_plot(FIELD_BFIELD_Z);
    stringstream file_prefix;
    file_prefix << "beam.png";
    geomplotter.plot_png(file_prefix.str());

    // IBSimu_plot = new Fl_PNG_Image("beam.png");
    // IBSimu_plot_box->image(IBSimu_plot);


    /////////////////////////////INITIALIZE PLOTTER////////////////////////////


    // plenv( 0.0, 1.0, 0.0, 1.0, 1, 0 );
    // pllab("x", "y", "Energy");
    // pls->w3d( 1.0, 1.0, 1.2, -3.0, 3.0, -3.0, 3.0, zmin, zmax,
    //     alt[k], az[k] );
    // pls->mesh( x, y, z, XPTS, YPTS, opt[k] | MAG_COLOR );
    /////////////////////////////INITIALIZE PLOTTER////////////////////////////

    //////////////////INITIALIZE RADIAL VELOCITY VECTOR PLOTTER//////////////
    PLINT narr;

    // Set of points making a polygon to use as the arrow
    PLFLT arrow_x[6]  = { -0.5, 0.5, 0.3, 0.5, 0.3, 0.5 };
    PLFLT arrow_y[6]  = { 0.0, 0.0, 0.2, 0.0, -0.2, 0.0 };

    pls = new plstream();

    cairo_t         *cairoContext;
    pls->setopt( "-geometry", "1000x1000" );
    pls->sfnam (	"diags.png" );
    pls->sdev( "pngcairo" );
    plssub( 2, 2 );

    pls->init();
    pls->cmd( PLESC_DEVINIT, cairoContext );

    pls->svect( arrow_x, arrow_y, narr, 0 ); //set arrow style


    pls->Alloc2dGrid( &particle_x_velocities,X_DIAGNOSTIC_COUNT, NUMBER_OF_PARTICLES );
    pls->Alloc2dGrid( &particle_y_velocities,X_DIAGNOSTIC_COUNT, NUMBER_OF_PARTICLES );
    //
    // PLcGrid2 cgrid2;
    // plAlloc2dGrid( &cgrid2.xg, X_DIAGNOSTIC_COUNT, NUMBER_OF_PARTICLES );
    // plAlloc2dGrid( &cgrid2.yg, X_DIAGNOSTIC_COUNT, NUMBER_OF_PARTICLES );
    //
    // cgrid2.nx = X_DIAGNOSTIC_COUNT;
    // cgrid2.ny = NUMBER_OF_PARTICLES;

    double final_particle_energies[NUMBER_OF_PARTICLES];
    double final_particle_energies_x[NUMBER_OF_PARTICLES];
    int final_particles_remaining = 0;
    vector<trajectory_diagnostic_e> diagnostics;
    diagnostics.push_back( DIAG_VR ); //first added is index 0, second is index 1, etc.
    diagnostics.push_back( DIAG_EK );
    diagnostics.push_back( DIAG_R );

    double max_radial_velocity[X_DIAGNOSTIC_COUNT];
    double beam_diagnostic_positions[X_DIAGNOSTIC_COUNT];

    for(int x_pos = 0; x_pos < X_DIAGNOSTIC_COUNT; x_pos+=1){

      TrajectoryDiagnosticData tdata;
      pdb.trajectories_at_plane( tdata, AXIS_X, x_pos*DIAGNOSTIC_X_INTERVAL, diagnostics );
      const TrajectoryDiagnosticColumn &diag_radial_position = tdata(2);
      const TrajectoryDiagnosticColumn &diag_energy = tdata(1);
      const TrajectoryDiagnosticColumn &diag_radial_velocity = tdata(0);
      particle_x_coords[x_pos] = x_pos*DIAGNOSTIC_X_INTERVAL;

      for(uint32_t i = 0; i < diag_radial_velocity.size(); i+=1) {
        particle_y_coords[i] = diag_radial_position(i);
        //
        // cgrid2.xg[x_pos][i] = x_pos*DIAGNOSTIC_X_INTERVAL;
        // cgrid2.yg[x_pos][i] = diag_radial_position(i);

        // printf("%i,%i\n",(int) (x_pos/DIAGNOSTIC_X_INTERVAL),i);
        // printf("%i,%i\n",X_DIAGNOSTIC_COUNT,NUMBER_OF_PARTICLES);

        particle_y_velocities[x_pos][i] = diag_radial_velocity(i);
        // particle_x_velocities[x_pos][i] = 100.0;
        if(diag_radial_velocity(i) > max_radial_velocity[x_pos]){
          max_radial_velocity[x_pos] = diag_radial_velocity(i);
        }
        beam_diagnostic_positions[x_pos] = x_pos*DIAGNOSTIC_X_INTERVAL;
      }
      if(x_pos > X_DIAGNOSTIC_COUNT-3){
        final_particles_remaining=diag_energy.size();
        printf("");
        for(uint32_t i = 0; i < diag_energy.size(); i+=1) {
          final_particle_energies[i] = diag_energy(i);
          final_particle_energies_x[i] = i;
        }
      }
    }
    for(uint32_t i = 0; i < NUMBER_OF_PARTICLES; i+=1) {
      particle_y_coords[i] = i;
    }
    ///cool array printer
    // for(auto const& value: final_particle_energies)
    // {
    //     std::cout << value << "\n";
    // }
    ///setup plotter

    //plot beam exit energy
    if(final_particles_remaining > 0){
    plcol0( 1 );
    pls->env(0.0,final_particles_remaining, 0.0,
                  *std::max_element(std::begin(final_particle_energies), std::begin(final_particle_energies)+final_particles_remaining)*1.2, 0, 0 );

    pls->col0( 2 );
    plline(final_particles_remaining, final_particle_energies_x,final_particle_energies);
    pls->col0( 1 );

    //plot beam velocity vectors
    plcol0( 1 );
    }
    // pls->env(0.0, MESH_X, 0.0, NUMBER_OF_PARTICLES, 0, 0);
    pls->lab( "particle no.", "eV","Beam exit energy");

    plcol0( 1 );
    pls->env(0.0,(*std::max_element(std::begin(beam_diagnostic_positions), std::end(beam_diagnostic_positions)))*1,
                    *std::min_element(std::begin(max_radial_velocity), std::end(max_radial_velocity)),
                  *std::max_element(std::begin(max_radial_velocity), std::end(max_radial_velocity))*1.2, 0, 0 );
    pls->col0( 2 );
    plline(X_DIAGNOSTIC_COUNT,beam_diagnostic_positions,max_radial_velocity);
    pls->col0( 1 );
    //plot beam velocity vectors
    plcol0( 1 );

    // pls->env(0.0, MESH_X, 0.0, NUMBER_OF_PARTICLES, 0, 0);
    pls->lab( "x position (m)", "m/s","Max radial velocity");



    pls->Free2dGrid(particle_x_velocities, ((MESH_X-GRID_SIZE)/DIAGNOSTIC_X_INTERVAL), NUMBER_OF_PARTICLES );
    pls->Free2dGrid(particle_y_velocities, ((MESH_X-GRID_SIZE)/DIAGNOSTIC_X_INTERVAL), NUMBER_OF_PARTICLES );

    plend();
    // delete[] x;
    // delete[] y;

    // pls->line( NSIZE, x, y );

    // pls->plot3d( x, y, z, XPTS, YPTS, opt[k] | MAG_COLOR, true );
    // beam_envelope = new Fl_PNG_Image("test.png");
    // beam_envelope_box->image(beam_envelope);


    iteration+=1;
    clock_t end = clock();
    float sim_duration = (float)(end - start) / CLOCKS_PER_SEC;

    window->redraw();

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
