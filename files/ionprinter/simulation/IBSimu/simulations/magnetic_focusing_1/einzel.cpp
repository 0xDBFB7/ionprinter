#include "epot_bicgstabsolver.hpp"
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

const double Te = 5.0;
const double Up = 5.0;


bool solid1( double x, double y, double z )
{
    return( x <= 0.00187 && y >= 0.00054 && y >= 2.28*x - 0.0010 &&
            (x >= 0.00054 || y >= 0.0015) );
}


bool solid2( double x, double y, double z )
{
    return( x >= 0.0095 && y >= 0.0023333 && y >= 0.01283 - x );
}


void simu( int *argc, char ***argv )
{
    Geometry geom( MODE_CYL, Int3D(200,200,1), Vec3D(0,0,0), 0.005 );
    // Solid *s1 = new FuncSolid( solid1 );
    // geom.set_solid( 7, s1 );
    // Solid *s2 = new FuncSolid( solid2 );
    // geom.set_solid( 8, s2 );
    geom.set_boundary( 1, Bound(BOUND_NEUMANN,    0.0 ) );
    geom.set_boundary( 2, Bound(BOUND_DIRICHLET, -12.0e3) );
    geom.set_boundary( 3, Bound(BOUND_NEUMANN,    0.0) );
    // geom.set_boundary( 4, Bound(BOUND_NEUMANN,    0.0) );
    //geom.set_boundary( 7, Bound(BOUND_DIRICHLET,  0.0)  );
    // geom.set_boundary( 8, Bound(BOUND_DIRICHLET, -12.0e3) );
    geom.build_mesh();

    EpotBiCGSTABSolver solver( geom );

    EpotField epot( geom );
    MeshScalarField scharge( geom );
    EpotEfield efield( epot );
    field_extrpl_e efldextrpl[6] = { FIELD_EXTRAPOLATE, FIELD_EXTRAPOLATE,
				     FIELD_SYMMETRIC_POTENTIAL, FIELD_EXTRAPOLATE,
				     FIELD_EXTRAPOLATE, FIELD_EXTRAPOLATE };
    efield.set_extrapolation( efldextrpl );

    bool fout[3] = { true, true, true };
    MeshVectorField bfield( geom, fout);
    for( int32_t i = 0; i < bfield.size(0); i++ ) {
        for( int32_t j = 0; j < bfield.size(1); j++ ) {

                bfield.set( i, j, 0, Vec3D( 1, 100, 100 ) );
        }
    }


    ParticleDataBaseCyl pdb( geom );
    bool pmirror[6] = { false, false, true, false, false, false };
    pdb.set_mirror( pmirror );

    for( size_t i = 0; i < 15; i++ ) {

    	solver.solve( epot, scharge );
    	efield.recalculate();

    	pdb.clear();
      float beam_area = 2.0*M_PI*pow(BEAM_RADIUS,2); //m^2

    	pdb.add_2d_beam_with_energy(
                                            100, //number of particles
                                            BEAM_CURRENT/beam_area, //beam current density
                                            1.0, //charge per particle
                                            29.0, //amu
                                            15, //eV
                                            1,//Normal temperature
                                            1,
                                            0,0, //point 1
                                            0,BEAM_RADIUS //point 2
                                            );

    	pdb.iterate_trajectories( scharge, efield, bfield );

    }

#ifdef GTK3
    GTKPlotter plotter( argc, argv );
    plotter.set_geometry( &geom );
    plotter.set_epot( &epot );
    plotter.set_bfield( &bfield );
    plotter.set_scharge( &scharge );
    plotter.set_particledatabase( &pdb );
    plotter.new_geometry_plot_window();
    plotter.run();
#endif
}


int main( int argc, char **argv )
{
    try {
        ibsimu.set_message_threshold( MSG_VERBOSE, 1 );
	ibsimu.set_thread_count( 4 );
        simu( &argc, &argv );
    } catch ( Error e ) {
	e.print_error_message( ibsimu.message( 0 ) );
        exit( 1 );
    }

    return( 0 );
}
