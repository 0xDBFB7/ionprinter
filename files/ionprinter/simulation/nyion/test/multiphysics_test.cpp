#include "multiphysics.hpp"
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"

// #include "QUnit.hpp"
#include <iostream>


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
}

TEST_GROUP(mesh_tests){};

TEST(mesh_tests,mesh_import_test_1){
  int mesh_geometry[3] = {20,1,1};
  float mesh_scale[3] = {0.001,0.001,0.001};
  std::vector<bool> mesh_present(10000);
  double bounds[6];
  float translate[3] = {0};
  import_mesh("../10x10x10_cube.stl",mesh_present,mesh_geometry,mesh_scale,bounds,translate);
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

TEST(mesh_tests,mesh_import_test_2){
  int mesh_geometry[3] = {20,1,1};
  float mesh_scale[3] = {0.001,0.001,0.001};
  std::vector<bool> mesh_present(10000);
  double bounds[6];
  float translate[3] = {0.1,0,0};
  import_mesh("../10x10x10_cube.stl",mesh_present,mesh_geometry,mesh_scale,bounds,translate);
  DOUBLES_EQUAL(0.1,bounds[0],1e-6);
  DOUBLES_EQUAL(0.11,bounds[1],1e-6);
  DOUBLES_EQUAL(0,bounds[2],1e-6);
  DOUBLES_EQUAL(0.01,bounds[3],1e-6);
  DOUBLES_EQUAL(0,bounds[4],1e-6);
  DOUBLES_EQUAL(0.01,bounds[5],1e-6);

  CHECK_EQUAL(true, mesh_present[f_idx(0.018,0.0005,0.0005,mesh_geometry,mesh_scale)]);
  CHECK_EQUAL(true, mesh_present[f_idx(0.01,0,0,mesh_geometry,mesh_scale)]);
  CHECK_EQUAL(false, mesh_present[f_idx(0.021,0,0,mesh_geometry,mesh_scale)]);

}

int main(int ac, char** av){
  return RUN_ALL_TESTS(ac, av);

    // int mesh_geometry[3];
    // QUNIT_IS_EQUAL(42, 41);

    // return qunit.errors();
}
