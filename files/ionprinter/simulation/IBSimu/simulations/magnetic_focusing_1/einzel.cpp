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


#ifdef GTK3
#include "gtkplotter.hpp"
#endif

#define BEAM_RADIUS 0.01 //m
#define BEAM_CURRENT 35.75 //A 35
#define BEAM_ENERGY 15 //eV

#define BEAM_OFFSET_Y 0.1

#define GRID_SIZE 0.001 //m
#define RECOMBINATION_POINT 0.23 //m

#define BFIELD_X 0.3
#define BFIELD_PEAK 10

#define MESH_LENGTH 0.7
#define MESH_WIDTH 0.2

#define INTERACTIVE_PLOT 1

const double Te = 5.0;
const double Up = 5.0;

int iteration = 0;

bool einzel_1( double x, double y, double z )
{
    return( y > 0.02 && x < 0.008);
}


bool einzel_2( double x, double y, double z )
{
    return( y > 0.02 && x > 0.03);
}


void simu( int *argc, char ***argv )
{
    while(iteration < 1){
    Geometry geom( MODE_2D, Int3D(MESH_LENGTH/GRID_SIZE,MESH_WIDTH/GRID_SIZE,1), Vec3D(0,0,0), GRID_SIZE );

    // Solid *s1 = new FuncSolid( einzel_1 );
    // geom.set_solid( 7, s1 );
    // Solid *s2 = new FuncSolid( einzel_2 );
    // geom.set_solid( 8, s2 );

    geom.set_boundary( 1, Bound(BOUND_NEUMANN,     0.0 ) );
    geom.set_boundary( 2, Bound(BOUND_DIRICHLET,  0.0) );
    geom.set_boundary( 3, Bound(BOUND_NEUMANN,     0.0) );
    geom.set_boundary( 4, Bound(BOUND_NEUMANN,     0.0) );
    // geom.set_boundary( 7, Bound(BOUND_DIRICHLET,  0.0) );
    // geom.set_boundary( 8, Bound(BOUND_DIRICHLET,  0.0) );
    // geom.set_boundary( 4, Bound(BOUND_NEUMANN,    0.0) );
    //geom.set_boundary( 7, Bound(BOUND_DIRICHLET,  0.0)  );
    // geom.set_boundary( 8, Bound(BOUND_DIRICHLET, -12.0e3) );
    geom.build_mesh();

    EpotUMFPACKSolver solver( geom );

    EpotField epot( geom );
    MeshScalarField scharge( geom );
    EpotEfield efield( epot );

    field_extrpl_e efldextrpl[6] = { FIELD_EXTRAPOLATE, FIELD_EXTRAPOLATE,
      FIELD_EXTRAPOLATE, FIELD_EXTRAPOLATE,
      FIELD_EXTRAPOLATE, FIELD_EXTRAPOLATE };
    efield.set_extrapolation( efldextrpl );


    ParticleDataBase2D pdb( geom );
    pdb.set_thread_count(10);
    bool pmirror[6] = { false, false, false, false, false, false };
    pdb.set_mirror( pmirror );

    bool fout[3] = { true, true, true };
    //MeshVectorField bfield( geom, fout);
    MeshVectorField bfield(MODE_2D, fout, 1.0, 1.0, "B.dat");


    //
    // for( int32_t x = 0; x < RECOMBINATION_POINT/GRID_SIZE; x++ ) {
    //   for( int32_t y = 0; y < bfield.size(1); y++ ) {
    //       //bfield.set( x, y, 0, Vec3D( 0, 0, (y/200.0)*5 ) );
    //       double gaussian_x = (BFIELD_PEAK*pow(2.71828,-1.0*(pow(x-200,2.0)/20000.0)));
    //       double gaussian_y = 0;
    //       if(y < bfield.size(1)/2){
    //         gaussian_y = -(pow(2.71828,-1.0*(pow(y+100,2.0)/20000.0)));
    //       }
    //       else{
    //         gaussian_y = (pow(2.71828,-1.0*(pow(300-y,2.0)/20000.0)));
    //       }
    //       bfield.set( x, y, 0, Vec3D( 0, 0,  gaussian_x*gaussian_y));
    //   }
    // }

    // for( int32_t x = 200; x < RECOMBINATION_POINT/GRID_SIZE; x++ ) {
    //   for( int32_t y = 0; y < bfield.size(1); y++ ) {
    //       //bfield.set( x, y, 0, Vec3D( 0, 0, (y/200.0)*5 ) );
    //       double gaussian_x = (BFIELD_PEAK*pow(2.71828,-1.0*(pow(x-100,2.0)/20000.0)));
    //       double gaussian_y = 0;
    //       if(y < bfield.size(1)/2){
    //         gaussian_y = -(pow(2.71828,-1.0*(pow(y+100,2.0)/20000.0)));
    //       }
    //       else{
    //         gaussian_y = (pow(2.71828,-1.0*(pow(300-y,2.0)/20000.0)));
    //       }
    //       bfield.set( x, y, 0, Vec3D( 0, 0,  gaussian_x*gaussian_y));
    //   }
    // }

    for( size_t i = 0; i < 15; i++ ) {

    	solver.solve( epot, scharge );

    	efield.recalculate();

    	pdb.clear();
      //float beam_area = 2.0*M_PI*pow(BEAM_RADIUS,2); //m^2

      float beam_area = BEAM_RADIUS*2; //m
      //see https://sourceforge.net/p/ibsimu/mailman/message/31283552/


    	pdb.add_2d_beam_with_energy(
                                            1000, //number of particles
                                            BEAM_CURRENT/beam_area, //beam current density
                                            1.0, //charge per particle
                                            29.0, //amu
                                            BEAM_ENERGY, //eV
                                            1,//Normal temperature
                                            1,
                                            0.005,BEAM_OFFSET_Y-BEAM_RADIUS, //point 1
                                            0.005,BEAM_OFFSET_Y+BEAM_RADIUS //point 2
                                            );

      // pdb.add_cylindrical_beam_with_energy(  1000, //number of particles
      //                                       BEAM_CURRENT/beam_area, //beam current density
      //                                       1.0, //charge per particle
      //                                       29.0, //amu
      //                                       BEAM_ENERGY, //eV
      //                                       1,//Normal temperature
      //                                       1,
      //                       					      Vec3D(0,0,0),
      //                       					      Vec3D(0,1,0),
      //                       					      Vec3D(0,0,1),
      //                       					      BEAM_RADIUS );

    	pdb.iterate_trajectories( scharge, efield, bfield );


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

  if(INTERACTIVE_PLOT){
    GTKPlotter plotter( argc, argv );
    plotter.set_geometry( &geom );
    plotter.set_epot( &epot );
    plotter.set_bfield( &bfield );
    plotter.set_scharge( &scharge );
    plotter.set_particledatabase( &pdb );
    plotter.new_geometry_plot_window();
    plotter.run();
  }
  GeomPlotter geomplotter( geom );
  geomplotter.set_size( MESH_LENGTH/GRID_SIZE+40, MESH_WIDTH/GRID_SIZE );
  geomplotter.set_epot( &epot );
  geomplotter.set_bfield( &bfield );
  geomplotter.set_particle_database( &pdb );
  geomplotter.set_fieldgraph_plot(FIELD_BFIELD_Z);
  std::stringstream fmt;
  fmt << "images/" << iteration << ".png";
  geomplotter.plot_png(fmt.str());

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
