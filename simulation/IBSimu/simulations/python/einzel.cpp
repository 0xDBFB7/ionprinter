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

#include <Python.h>

#include <vector>
#include <stdexcept>

#include <unistd.h>


using namespace std;


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
float feature_1_voltage = 200;
float feature_2_voltage = 0;
float feature_3_voltage = 200;

float recombination_point = MESH_X;

int iteration = 0;
int run_id = 1;


// void simu( int *argc, char ** argv )
// {
//
//       ibsimu.set_message_threshold( MSG_VERBOSE, 1 );
//     ibsimu.set_thread_count( 10 );
//
//       Geometry geom( MODE_CYL, Int3D(MESH_X/GRID_SIZE,MESH_Y/GRID_SIZE,1), Vec3D(0,0,0), GRID_SIZE );
//
//
//       geom.set_boundary( 1, Bound(BOUND_NEUMANN,     0.0 ) );
//       geom.set_boundary( 2, Bound(BOUND_DIRICHLET,  0.0) );
//       geom.set_boundary( 3, Bound(BOUND_NEUMANN,     0.0) );
//       geom.set_boundary( 4, Bound(BOUND_NEUMANN,     0.0) );
//       // for(int n : feature_points) {
//       //   Solid *s1 = new FuncSolid( feature_1 );
//       //   geom.set_solid(n+7, s1 );
//       //   geom.set_boundary(n+7, Bound(BOUND_DIRICHLET,  feature_voltages[n]) );
//       // }
//       geom.build_mesh();
//
//       EpotUMFPACKSolver solver( geom );
//       //EpotBiCGSTABSolver solver( geom );
//
//       EpotField epot( geom );
//       MeshScalarField scharge( geom );
//       EpotEfield efield( epot );
//
//       field_extrpl_e efldextrpl[6] = { FIELD_EXTRAPOLATE, FIELD_EXTRAPOLATE,
//         FIELD_SYMMETRIC_POTENTIAL, FIELD_EXTRAPOLATE,
//         FIELD_EXTRAPOLATE, FIELD_EXTRAPOLATE };
//       efield.set_extrapolation( efldextrpl );
//
//       ParticleDataBaseCyl pdb( geom );
//       pdb.set_thread_count(10);
//       bool pmirror[6] = { false, false, false, false, false, false };
//       pdb.set_mirror( pmirror );
//
//       bool fout[3] = { true, true, true };
//       MeshVectorField bfield( geom, fout);
//
//
//
//       for( size_t i = 0; i < 15; i++ ) {
//       	solver.solve( epot, scharge );
//       	efield.recalculate();
//       	pdb.clear();
//
//         //float beam_area = BEAM_RADIUS*2; //m
//         //see https://sourceforge.net/p/ibsimu/mailman/message/31283552/
//
//         float beam_area = (M_PI*pow(beam_radius,2));
//         printf("Beam_area: %f\n",beam_area);
//       	pdb.add_2d_beam_with_energy(
//                                               NUMBER_OF_PARTICLES, //number of particles
//                                               beam_current/beam_area, //beam current density
//                                               1.0, //charge per particle
//                                               29, //amu
//                                               beam_input_energy, //eV
//                                               0.2,//Normal temperature
//                                               0.1,
//                                               beam_x_position,0, //point 1
//                                               beam_x_position,0+beam_radius //point 2
//                                               );
//
//         float ion_curtain_area = 2*M_PI*ION_CURTAIN_WIRE_RADIUS*ION_CURTAIN_WIRE_WIDTH;
//
//         // pdb.add_2d_beam_with_energy(
//         //                                       1000, //number of particles
//         //                                       BEAM_CURRENT/beam_area, //beam current density
//         //                                       -1.0, //charge per particle
//         //                                       0.000548, //amu
//         //                                       ION_CURTAIN_ENERGY, //eV
//         //                                       0.3,//Normal temperature
//         //                                       0.1,
//         //                                       0.0015+ION_CURTAIN_WIRE_WIDTH,ION_CURTAIN_WIRE_RADIUS, //point 1
//         //                                       0.0015,ION_CURTAIN_WIRE_RADIUS //point 2
//         //                                       );
//         //
//       	pdb.iterate_trajectories( scharge, efield, bfield );
//
//         //Make a histogram of particle energies
//         //https://sourceforge.net/p/ibsimu/mailman/message/28374280/
//
//
//
//         for( int ii = 0; ii < epot.size(0); ii++ ) {
//           for( int jj = 0; jj < epot.size(1); jj++ ) {
//             for( int kk = 0; kk < epot.size(2); kk++ ) {
//               // double z = scharge.h()*kk+scharge.origo(2);
//                if(ii > recombination_point/GRID_SIZE) {
//                  epot(ii,jj,kk) = 0;
//                }
//             }
//           }
//         }
//
//         //Rather than adjust the charge of the particles after recombination,
//         //I'm nulling out the space charge field.
//         //'s easier, ya see.
//         //code block taken from the IBSimu mailing list.
//         for( int ii = 0; ii < scharge.size(0); ii++ ) {
//           for( int jj = 0; jj < scharge.size(1); jj++ ) {
//             for( int kk = 0; kk < scharge.size(2); kk++ ) {
//               // double z = scharge.h()*kk+scharge.origo(2);
//                if(ii > recombination_point/GRID_SIZE) {
//                 scharge(ii,jj,kk) = 0;
//                }
//             }
//           }
//         }
//       }
//
//
//
//
//     // vector<trajectory_diagnostic_e> diagnostics;
//     // diagnostics.push_back( DIAG_EK );
//     // diagnostics.push_back( DIAG_X );
//     // TrajectoryDiagnosticData tdata;
//     // pdb.trajectories_at_plane( tdata, AXIS_X, 0.0001, diagnostics );
//     // const TrajectoryDiagnosticColumn &energy = tdata(1);
//     // const TrajectoryDiagnosticColumn &energy = tdata(0);
//     //
//     // for( uint32_t i = 0; i < energy.size(); i++ ) {
//     //   of_trajdiag << energy(i) << " "
//     // }
//     //
//     // Histogram1D energy_histo( 20, energy.data(), current.data() );
//     // vector<double> energy_data = energy_histo.get_data();
//     // for( uint32_t i = 0; i < energy_histo.n(); i++ ) {
//     //    energy_histo.coord(i) << " " << energy_histo(i) << "\n";
//     // }
//     //
//     // of_histo.close();
//     // double range[2];
//     // energy_histo.get_range( range );
//     //
//
//
//     vector<trajectory_diagnostic_e> diagnostics;
//     diagnostics.push_back( DIAG_VR ); //first added is index 0, second is index 1, etc.
//     diagnostics.push_back( DIAG_EK );
//     diagnostics.push_back( DIAG_R );
//
//     for(float x_pos = 0; x_pos < (MESH_X-GRID_SIZE)-1; x_pos+=DIAGNOSTIC_X_INTERVAL){
//
//       TrajectoryDiagnosticData tdata;
//       pdb.trajectories_at_plane( tdata, AXIS_X, x_pos, diagnostics );
//       const TrajectoryDiagnosticColumn &diag_radial_position = tdata(2);
//       const TrajectoryDiagnosticColumn &diag_energy = tdata(1);
//       const TrajectoryDiagnosticColumn &diag_radial_velocity = tdata(0);
//
//       for(uint32_t i = 0; i < diag_radial_velocity.size(); i+=1) {
//         particle_y_coords[(int) (x_pos/DIAGNOSTIC_X_INTERVAL)][i] = diag_radial_position(i);
//         printf("%i,%i\n",(int) (x_pos/DIAGNOSTIC_X_INTERVAL),i);
//         // printf("%i,%i\n",X_DIAGNOSTIC_COUNT,NUMBER_OF_PARTICLES);
//         particle_y_velocities[(int)(x_pos/(DIAGNOSTIC_X_INTERVAL))][i] = diag_radial_velocity(i);
//       }
//     }
//
//   }
// }
// from examples/fltk_example.cpp

PyObject *p1;


static PyObject* run_simulation(){

  ibsimu.set_message_threshold( MSG_VERBOSE, 1 );
  ibsimu.set_thread_count( 10 );

  Geometry geom( MODE_CYL, Int3D(MESH_X/GRID_SIZE,MESH_Y/GRID_SIZE,1), Vec3D(0,0,0), GRID_SIZE );

  geom.set_boundary( 1, Bound(BOUND_NEUMANN,     0.0 ) );
  geom.set_boundary( 2, Bound(BOUND_DIRICHLET,  0.0) );
  geom.set_boundary( 3, Bound(BOUND_NEUMANN,     0.0) );
  geom.set_boundary( 4, Bound(BOUND_NEUMANN,     0.0) );

  // for(int n : feature_points) {
  //   Solid *s1 = new FuncSolid( feature_1 );
  //   geom.set_solid(n+7, s1 );
  //   geom.set_boundary(n+7, Bound(BOUND_DIRICHLET,  feature_voltages[n]) );
  // }

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

    vector<trajectory_diagnostic_e> diagnostics;
    diagnostics.push_back( DIAG_VR ); //first added is index 0, second is index 1, etc.
    diagnostics.push_back( DIAG_EK );
    diagnostics.push_back( DIAG_R );

    for(float x_pos = 0; x_pos < (MESH_X-GRID_SIZE)-1; x_pos+=DIAGNOSTIC_X_INTERVAL){

      TrajectoryDiagnosticData tdata;
      pdb.trajectories_at_plane( tdata, AXIS_X, x_pos, diagnostics );
      const TrajectoryDiagnosticColumn &diag_radial_position = tdata(2);
      const TrajectoryDiagnosticColumn &diag_energy = tdata(1);
      const TrajectoryDiagnosticColumn &diag_radial_velocity = tdata(0);

      for(uint32_t i = 0; i < diag_radial_velocity.size(); i+=1) {
        particle_y_coords[(int) (x_pos/DIAGNOSTIC_X_INTERVAL)][i] = diag_radial_position(i);
        printf("%i,%i\n",(int) (x_pos/DIAGNOSTIC_X_INTERVAL),i);
        // printf("%i,%i\n",X_DIAGNOSTIC_COUNT,NUMBER_OF_PARTICLES);
        particle_y_velocities[(int)(x_pos/(DIAGNOSTIC_X_INTERVAL))][i] = diag_radial_velocity(i);
      }
    }
  }
  PyObject *d = PyDict_New()
  for (...) {
    PyDict_SetItem(d, key, val);
  }
  return d;
}

static PyObject* simulate(PyObject *self, PyObject *args) {
    const char* name;
    if (!PyArg_ParseTuple(args, "s", &name)) {
        return NULL;
    }


    Py_RETURN_NONE;
}

//from https://gist.github.com/physacco/2e1b52415f3a964ad2a542a99bebed8f - thanks physacco!

// Method definition object for this extension, these argumens mean:
// ml_name: The name of the method
// ml_meth: Function pointer to the method implementation
// ml_flags: Flags indicating special features of this method, such as
//          accepting arguments, accepting keyword arguments, being a
//          class method, or being a static method of a class.
// ml_doc:  Contents of this method's docstring
static PyMethodDef simulation_methods[] = {
    {
        "simulate", simulate, METH_VARARGS,
        "Run an ion beam simulation"
    },
    {
        "run_simulation", run_simulation, METH_VARARGS,
        "Run an ion beam simulation"
    },
    {NULL, NULL, 0, NULL}
};

// Module definition
// The arguments of this structure tell Python what to call your extension,
// what it's methods are and where to look for it's method definitions
static struct PyModuleDef simulate_definition = {
    PyModuleDef_HEAD_INIT,
    "IBSimulate",
    "A Python module that prints 'hello world' from C code.",
    -1,
    simulation_methods
};

// Module initialization
// Python calls this function when importing your extension. It is important
// that this function is named PyInit_[[your_module_name]] exactly, and matches
// the name keyword argument in setup.py's setup() call.
PyMODINIT_FUNC PyInit_IBSimulate(void) {
    Py_Initialize();
    return PyModule_Create(&simulate_definition);
}
