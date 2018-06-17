#include "epot_bicgstabsolver.hpp"
#include "geometry.hpp"
#include "func_solid.hpp"
#include "epot_efield.hpp"
#include "meshvectorfield.hpp"
#include "particledatabase.hpp"
#include "geomplotter.hpp"
#include "ibsimu.hpp"
#include "error.hpp"


bool solid1( double x, double y, double z )
{
    return( x <= 0.02 && y >= 0.018 );
}


bool solid2( double x, double y, double z )
{
    return( x >= 0.03 && x <= 0.04 && y >= 0.02 );
}


bool solid3( double x, double y, double z )
{
    return( x >= 0.06 && y >= 0.03 && y >= 0.07 - 0.5*x );
}


void simu( void )
{
    Geometry geom( MODE_2D, Int3D(241,101,1), Vec3D(0,0,0), 0.0005 );

    Solid *s1 = new FuncSolid( solid1 );
    geom.set_solid( 7, s1 );
    Solid *s2 = new FuncSolid( solid2 );
    geom.set_solid( 8, s2 );
    Solid *s3 = new FuncSolid( solid3 );
    geom.set_solid( 9, s3 );
    
    geom.set_boundary( 1, Bound(BOUND_DIRICHLET,  0.0e3) );
    geom.set_boundary( 2, Bound(BOUND_DIRICHLET,  0.0e3) );
    geom.set_boundary( 3, Bound(BOUND_NEUMANN,     0.0  ) );
    geom.set_boundary( 4, Bound(BOUND_NEUMANN,     0.0  ) );
    geom.set_boundary( 7, Bound(BOUND_DIRICHLET,  -3.0e3) );
    geom.set_boundary( 8, Bound(BOUND_DIRICHLET, -14.0e3) );
    geom.set_boundary( 9, Bound(BOUND_DIRICHLET,  -1.0e3) );
    geom.build_mesh();

    EpotField epot( geom );
    MeshScalarField scharge( geom );
    MeshVectorField bfield;
    EpotEfield efield( epot );
    field_extrpl_e efldextrpl[6] = { FIELD_EXTRAPOLATE, FIELD_EXTRAPOLATE, 
				     FIELD_SYMMETRIC_POTENTIAL, FIELD_EXTRAPOLATE,
				     FIELD_EXTRAPOLATE, FIELD_EXTRAPOLATE };
    efield.set_extrapolation( efldextrpl );

    EpotBiCGSTABSolver solver( geom );

    ParticleDataBase2D pdb( geom );
    bool pmirror[6] = { false, false, true, false, false, false };
    pdb.set_mirror( pmirror );

    for( size_t i = 0; i < 5; i++ ) {
	solver.solve( epot, scharge );
	efield.recalculate();
	pdb.clear();
	pdb.add_2d_beam_with_energy( 1000, 5000000, 1.0, 1.0, 
				     5000, 0.0, 0.0, 
				     0.0, 0.0, 
				     0.0, 0.0005 );
	pdb.iterate_trajectories( scharge, efield, bfield );
    }

    GeomPlotter geomplotter( geom );
    geomplotter.set_size( 750, 750 );
    geomplotter.set_epot( &epot );
    geomplotter.set_particle_database( &pdb );
    geomplotter.plot_png( "plot1.png" );
}


int main( int argc, char **argv )
{
    try {
	ibsimu.set_message_threshold( MSG_VERBOSE, 1 );
	ibsimu.set_thread_count( 4 );
        simu();
    } catch ( Error e ) {
        e.print_error_message( ibsimu.message( 0 ) );
        exit( 1 );
    }

    return( 0 );
}
