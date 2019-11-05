#include "epot_bicgstabsolver.hpp"
#include "geometry.hpp"
#include "func_solid.hpp"
#include "epot_efield.hpp"
#include "meshvectorfield.hpp"
#include "particledatabase.hpp"
#include "geomplotter.hpp"
#include "ibsimu.hpp"
#include "error.hpp"

#define EINZEL_EXTRA_R 0.005
#define SIM_EXTRA_R 0.025
#define BEAM_Y_TOP 0.051
#define BEAM_Y_BOTTOM 0.049

bool einzel_1( double x, double y, double z )
{
    return( x <= 0.05 && (y >= (BEAM_Y_TOP+EINZEL_EXTRA_R) || y <= (BEAM_Y_BOTTOM-EINZEL_EXTRA_R) ));
}


bool einzel_2( double x, double y, double z )
{
    return( x >= 0.06 && x <= 0.11 && (y >= (BEAM_Y_TOP+EINZEL_EXTRA_R) || y <= (BEAM_Y_BOTTOM-EINZEL_EXTRA_R) ));
}

bool einzel_3( double x, double y, double z )
{
    return( x >= 0.12 && x <= 0.2 && (y >= (BEAM_Y_TOP+EINZEL_EXTRA_R) || y <= (BEAM_Y_BOTTOM-EINZEL_EXTRA_R) ));
}

void simu( void )
{
    double sim_res = 0.0005; //m
    double sim_width = BEAM_Y_TOP+EINZEL_EXTRA_R+SIM_EXTRA_R; //m
    double sim_length = 0.6; //m
    Geometry geom( MODE_2D, Int3D(sim_length/sim_res,sim_width/sim_res,1), Vec3D(0,0,0), sim_res);

    Solid *s1 = new FuncSolid( einzel_1 );
    geom.set_solid( 7, s1 );
    Solid *s2 = new FuncSolid( einzel_2 );
    geom.set_solid( 8, s2 );
    Solid *s3 = new FuncSolid( einzel_3 );
    geom.set_solid( 9, s3 );
    
    geom.set_boundary( 1, Bound(BOUND_DIRICHLET,  0.0) );
    geom.set_boundary( 2, Bound(BOUND_DIRICHLET,  0.0) );
    geom.set_boundary( 3, Bound(BOUND_NEUMANN,     0.0  ) );
    geom.set_boundary( 4, Bound(BOUND_NEUMANN,     0.0  ) );
    geom.set_boundary( 7, Bound(BOUND_DIRICHLET,  0.0) );
    geom.set_boundary( 8, Bound(BOUND_DIRICHLET,  -0.0) );
    geom.set_boundary( 9, Bound(BOUND_DIRICHLET,  0.0) );
    geom.build_mesh();

    EpotField epot( geom );
    MeshScalarField scharge( geom );
    EpotEfield efield( epot );
    field_extrpl_e efldextrpl[6] = { FIELD_EXTRAPOLATE, FIELD_EXTRAPOLATE, 
				     FIELD_SYMMETRIC_POTENTIAL, FIELD_EXTRAPOLATE,
				     FIELD_EXTRAPOLATE, FIELD_EXTRAPOLATE };
    efield.set_extrapolation( efldextrpl );
    double Bz = 0.0;
    const bool fout[3] = {false, false, true};
    MeshVectorField extbfield(MODE_2D, fout, Int3D(100,100,1),Vec3D(0.00,0.00,0), 0.0005);
    for( uint32_t i = 0; i < extbfield.size(0); i++ ) {
        //double x = extbfield.origo(0)+i*bfield.h();
        for( uint32_t j = 0; j < extbfield.size(1); j++ ) {
            //double y = extbfield.origo(1)+j*bfield.h();
            for ( uint32_t k = 0; k < extbfield.size(2); k++ ) {
                //double z = extbfield.origo(2)+k*bfield.h();
		

		if(i < 50){
	                Bz = 0.0; //0.5 T
		}
		else{
	                Bz = -0.0; //-0.5 T
		}
                extbfield.set( i, j, k, Vec3D( 0, 0, Bz ) );
            }
        }
    }



    EpotBiCGSTABSolver solver( geom );

    ParticleDataBase2D pdb( geom );
    bool pmirror[6] = { false, false, true, false, false, false };
    pdb.set_mirror( pmirror );

    for( size_t i = 0; i < 5; i++ ) {
	solver.solve( epot, scharge );
	efield.recalculate();
	pdb.clear();
	pdb.add_2d_beam_with_energy( 1000, //beam traces 
					 50, //beam current, A/m
					 1.0, //particle charge, q
					 29.0, //mass in AMU of the particles 
					 5, //Energy of the particles, EV 
					 0.0,
					 0.0, 
					 0.0,
					 BEAM_Y_BOTTOM, 
					 0.0, 
					 BEAM_Y_TOP );
	pdb.set_max_steps(100);
	pdb.iterate_trajectories( scharge, efield, extbfield );	
	//pdb.step_particles(scharge, efield,0.00001);
    }
    GeomPlotter geomplotter( geom );
    geomplotter.set_size( 1500, 300 );
    geomplotter.set_epot( &epot );
    geomplotter.set_bfield( &extbfield );
    geomplotter.set_particle_database( &pdb );
    geomplotter.set_fieldgraph_plot(FIELD_BFIELD_Z);
    geomplotter.plot_svg( "plot1.svg" );
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
