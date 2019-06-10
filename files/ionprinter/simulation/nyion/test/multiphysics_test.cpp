#include "multiphysics.hpp"
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"



TEST_GROUP(idx_tests){};

TEST(idx_tests, i_idx_test)
{
  int mesh_geometry[3] = {10,10,10};
  CHECK_EQUAL(1, i_idx(1,0,0,mesh_geometry));
  CHECK_EQUAL(10, i_idx(0,1,0,mesh_geometry));
  CHECK_EQUAL(100, i_idx(0,0,1,mesh_geometry));
  CHECK_EQUAL(111, i_idx(1,1,1,mesh_geometry));
}

TEST(idx_tests, f_idx_test)
{
  int mesh_geometry[3] = {10,10,10};
  float mesh_scale[3] = {0.1,0.1,0.1};

  CHECK_EQUAL(111, f_idx(0.1,0.1,0.1,mesh_geometry,mesh_scale));
  CHECK_EQUAL(1110, f_idx(1,1,1,mesh_geometry,mesh_scale));

  std::vector<bool> mesh_present(i_idx(mesh_geometry[X],mesh_geometry[Y],mesh_geometry[Z],mesh_geometry));
  CHECK_EQUAL(1110, mesh_present.size());
}

TEST_GROUP(mesh_tests){};

TEST(mesh_tests,mesh_import_test_1){
  int mesh_geometry[3] = {20,1,1};
  float mesh_scale[3] = {0.001,0.001,0.001};
  std::vector<bool> mesh_present(i_idx(mesh_geometry[X],mesh_geometry[Y],mesh_geometry[Z],mesh_geometry));
  double bounds[6];
  // float translate[3] = {0};
  import_mesh("../10x10x10_cube.stl",mesh_present,mesh_geometry,mesh_scale,bounds);
  DOUBLES_EQUAL(0,bounds[0],1e-6);
  DOUBLES_EQUAL(0.01,bounds[1],1e-6);
  DOUBLES_EQUAL(0,bounds[2],1e-6);
  DOUBLES_EQUAL(0.01,bounds[3],1e-6);
  DOUBLES_EQUAL(0,bounds[4],1e-6);
  DOUBLES_EQUAL(0.01,bounds[5],1e-6);

  CHECK_EQUAL(true, mesh_present[f_idx(0.008,0,0,mesh_geometry,mesh_scale)]);
  CHECK_EQUAL(true, mesh_present[f_idx(0,0,0,mesh_geometry,mesh_scale)]);
  CHECK_EQUAL(false, mesh_present[f_idx(0.011,0,0,mesh_geometry,mesh_scale)]);
}

TEST_GROUP(laplace_tests){};

TEST(laplace_tests,laplace_tests_1){
  int mesh_geometry[3] = {20,3,3};
  std::vector<int> boundaries(i_idx(mesh_geometry[X],mesh_geometry[Y],mesh_geometry[Z],mesh_geometry),0);
  std::vector<bool> active(i_idx(mesh_geometry[X],mesh_geometry[Y],mesh_geometry[Z],mesh_geometry),0);
  std::vector<float> potentials(i_idx(mesh_geometry[X],mesh_geometry[Y],mesh_geometry[Z],mesh_geometry),0.0);

  potentials[i_idx(1,1,1,mesh_geometry)] = 10;
  boundaries[i_idx(1,1,1,mesh_geometry)] = 10;

  potentials[i_idx(5,1,1,mesh_geometry)] = 10;
  boundaries[i_idx(5,1,1,mesh_geometry)] = 10;

  relax_laplace_potentials(potentials, boundaries, mesh_geometry[X], mesh_geometry[Y], mesh_geometry[Z], 0.01);

  // std::copy(potentials.begin(), potentials.end(), std::ostream_iterator<float>(std::cout, " "));

  DOUBLES_EQUAL(0.58823, potentials[i_idx(3,1,1,mesh_geometry)],1e-3);
}


TEST(laplace_tests,laplace_timing_1){
  int mesh_geometry[3] = {200,200,200};

  std::vector<int> boundaries(i_idx(mesh_geometry[X],mesh_geometry[Y],mesh_geometry[Z],mesh_geometry),0);
  std::vector<bool> active(i_idx(mesh_geometry[X],mesh_geometry[Y],mesh_geometry[Z],mesh_geometry),1);
  std::vector<float> potentials(i_idx(mesh_geometry[X],mesh_geometry[Y],mesh_geometry[Z],mesh_geometry),0.0);

  potentials[i_idx(5,1,1,mesh_geometry)] = 10;
  boundaries[i_idx(5,1,1,mesh_geometry)] = 10;

  // auto t1 = std::chrono::high_resolution_clock::now();
  relax_laplace_potentials(potentials, boundaries, mesh_geometry[X], mesh_geometry[Y], mesh_geometry[Z], 0.01);

  // auto t2 = std::chrono::high_resolution_clock::now();
  // std::cout << "f() took " << (std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count())/10.0 << " milliseconds" << iterations << "\n";

  // std::copy(potentials.begin(), potentials.end(), std::ostream_iterator<float>(std::cout, " "));

  // DOUBLES_EQUAL(0.58823, potentials[i_idx(3,1,1,mesh_geometry)],1e-3);
}


//
// TEST(mesh_tests,mesh_import_test_2){
//
//   int mesh_geometry[3] = {1000,1,1};
//   float mesh_scale[3] = {0.001,0.001,0.001};
//   std::vector<bool> mesh_present(i_idx(mesh_geometry[X],mesh_geometry[Y],mesh_geometry[Z],mesh_geometry));
//   double bounds[6];
//   float translate[3] = {0.1,0,0};
//   import_mesh("../10x10x10_cube.stl",mesh_present,mesh_geometry,mesh_scale,bounds,translate);
//   DOUBLES_EQUAL(0.1,bounds[0],1e-6);
//   DOUBLES_EQUAL(0.11,bounds[1],1e-6);
//   DOUBLES_EQUAL(0,bounds[2],1e-6);
//   DOUBLES_EQUAL(0.01,bounds[3],1e-6);
//   DOUBLES_EQUAL(0,bounds[4],1e-6);
//   DOUBLES_EQUAL(0.01,bounds[5],1e-6);
//
//   for(unsigned int i=0; i<mesh_present.size(); ++i)
//     std::cout << mesh_present[i];
//
//   CHECK_EQUAL(true, mesh_present[f_idx(0.018,0.0005,0.0005,mesh_geometry,mesh_scale)]);
//   CHECK_EQUAL(true, mesh_present[f_idx(0.01,0,0,mesh_geometry,mesh_scale)]);
//   CHECK_EQUAL(false, mesh_present[f_idx(0.021,0,0,mesh_geometry,mesh_scale)]);
//
// }




int main(int ac, char** av){
  return RUN_ALL_TESTS(ac, av);

    // int mesh_geometry[3];
    // QUNIT_IS_EQUAL(42, 41);

    // return qunit.errors();
}
