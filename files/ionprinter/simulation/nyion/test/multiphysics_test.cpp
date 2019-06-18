#include "multiphysics.hpp"
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"



TEST_GROUP(idx_tests){};

// TEST(idx_tests, i_idx_test)
// {
//   int mesh_geometry[3] = {10,10,10};
//   CHECK_EQUAL(1, i_idx(1,0,0,mesh_geometry));
//   CHECK_EQUAL(10, i_idx(0,1,0,mesh_geometry));
//   CHECK_EQUAL(100, i_idx(0,0,1,mesh_geometry));
//   CHECK_EQUAL(111, i_idx(1,1,1,mesh_geometry));
// }


TEST(idx_tests, geometry_construct_test)
{
  float mesh_bounds[6] = {0.1,0.3,0.05,0.1,0,0.5};

  root_mesh_geometry test(mesh_bounds, 0.003, 0.000051);

  CHECK_EQUAL(67, test.root_x_len);
  CHECK_EQUAL(17, test.root_y_len);
  CHECK_EQUAL(167, test.root_z_len);

  DOUBLES_EQUAL(0.003, test.root_scale,1e-6);

  DOUBLES_EQUAL(0.1, test.x_min_bound,1e-6);
  DOUBLES_EQUAL(0.301, test.x_max_bound,1e-6);
  DOUBLES_EQUAL(0.05, test.y_min_bound,1e-6);
  DOUBLES_EQUAL(0.101, test.y_max_bound,1e-6);
  DOUBLES_EQUAL(0.0, test.z_min_bound,1e-6);
  DOUBLES_EQUAL(0.501, test.z_max_bound,1e-6);

  CHECK_EQUAL(58, test.sub_len);
  DOUBLES_EQUAL(0.0000517, test.sub_scale,1e-6);

}


TEST(idx_tests, vector_idx_test)
{
  float mesh_bounds[6] = {0.1,0.3,0.05,0.1,0,0.5};

  root_mesh_geometry test(mesh_bounds, 0.003, 0.000051);


}


TEST_GROUP(laplace_tests){};

TEST(laplace_tests,coarsen){
  float mesh_bounds[6] = {0,0.1,0,0.1,0,0.1};

  root_mesh_geometry mesh_geometry(mesh_bounds, 0.003, 0.00005);

  std::vector<std::vector<float>> potentials;
  std::vector<std::vector<int>> boundaries;

  potentials.resize(mesh_geometry.root_size);

  float mesh_active_bounds[6] = {0,0.006,0,0.006,0,0.006};

  enable_mesh_region(potentials,mesh_active_bounds,mesh_geometry);

  for(int x = 0; x < mesh_geometry.virtual_x_len; x++){
    set_mesh_value(x*10.0+100,x,0,0,potentials,mesh_geometry);
  }

  std::vector<std::vector<float>> coarsened_potentials;
  std::vector<std::vector<int>> coarsened_boundaries;

  root_mesh_geometry new_geometry = coarsen_mesh(potentials,coarsened_potentials,mesh_geometry,0.0005);

  // for(int x = 0; x < new_geometry.sub_len; x++){
  //   printf("%f\n",get_mesh_value(x,0,0,coarsened_potentials,new_geometry));
  // }

  CHECK_EQUAL(100.0, get_mesh_value(0,0,0,coarsened_potentials,new_geometry));
  CHECK_EQUAL(150.0, get_mesh_value(1,0,0,coarsened_potentials,new_geometry));



}


TEST(laplace_tests,decoarsen){
  float mesh_bounds[6] = {0,0.1,0,0.1,0,0.1};

  root_mesh_geometry mesh_geometry(mesh_bounds, 0.003, 0.00005);

  std::vector<std::vector<float>> potentials;
  std::vector<std::vector<int>> boundaries;

  potentials.resize(mesh_geometry.root_size);

  float mesh_active_bounds[6] = {0,0.006,0,0.006,0,0.006};

  enable_mesh_region(potentials,mesh_active_bounds,mesh_geometry);

  std::vector<std::vector<float>> decoarsened_potentials = potentials;

  for(int x = 0; x < mesh_geometry.virtual_x_len; x++){
        set_mesh_value(x,x,0,0,potentials,mesh_geometry);
  }

  std::vector<std::vector<float>> coarsened_potentials;
  std::vector<std::vector<int>> coarsened_boundaries;

  root_mesh_geometry new_geometry = coarsen_mesh(potentials,coarsened_potentials,mesh_geometry,0.0005);

  // for(int x = 0; x < new_geometry.sub_len; x++){
  //   printf("%f\n",get_mesh_value(x,0,0,coarsened_potentials,new_geometry));
  // }


  decoarsen_mesh(decoarsened_potentials,coarsened_potentials,mesh_geometry,new_geometry);

  // for(int x = 0; x < mesh_geometry.sub_len; x++){
  //   printf("%f\n",get_mesh_value(x,0,0,decoarsened_potentials,mesh_geometry));
  // }


  CHECK_EQUAL(100.0, get_mesh_value(0,0,0,coarsened_potentials,new_geometry));
  CHECK_EQUAL(150.0, get_mesh_value(1,0,0,coarsened_potentials,new_geometry));



}


TEST(laplace_tests,get_mesh){
  float mesh_bounds[6] = {0,0.1,0,0.1,0,0.1};

  root_mesh_geometry mesh_geometry(mesh_bounds, 0.003, 0.00005);

  std::vector<std::vector<float>> potentials;
  std::vector<std::vector<int>> boundaries;

  potentials.resize(mesh_geometry.root_size);

  float mesh_active_bounds[6] = {0,0.02,0,0.01,0,0.01};

  enable_mesh_region(potentials,mesh_active_bounds,mesh_geometry);

  set_mesh_value(1000.0,150,10,10,potentials,mesh_geometry);

  CHECK_EQUAL(1000.0,get_mesh_value(150,10,10,potentials,mesh_geometry));


}


TEST(laplace_tests,activate_submesh){

  float mesh_bounds[6] = {0,0.1,0,0.1,0,0.1};

  root_mesh_geometry mesh_geometry(mesh_bounds, 0.003, 0.00005);

  std::vector<std::vector<float>> potentials;
  std::vector<std::vector<int>> boundaries;

  potentials.resize(mesh_geometry.root_size);

  float mesh_active_bounds[6] = {0.003,0.006,0,0.003,0,0.003};



  enable_mesh_region(potentials,mesh_active_bounds,mesh_geometry);

  CHECK_EQUAL(0,potentials[0].size());
  CHECK_EQUAL(mesh_geometry.sub_size,potentials[1].size());
  CHECK_EQUAL(0,potentials[2].size());

  // DOUBLES_EQUAL(0.58823, potentials[10][i_idx(3,1,1,mesh_geometry)], 1e-2);
}

//
//
// TEST(laplace_tests,laplace_convergence_1){
//
//
//   float mesh_bounds[6] = {0,0.1,0,0.1,0,0.1};
//
//   root_mesh_geometry mesh_geometry(mesh_bounds, 0.003, 0.00005);
//
//   std::vector<std::vector<float>> potentials;
//   std::vector<std::vector<int>> boundaries;
//
//   potentials.resize(mesh_geometry.root_size);
//   boundaries.resize(mesh_geometry.root_size);
//
//   float mesh_active_bounds[6] = {0,0.01,0,0.01,0,0.01};
//
//   enable_mesh_region(potentials,mesh_active_bounds,mesh_geometry);
//   enable_mesh_region(boundaries,mesh_active_bounds,mesh_geometry);
//
//   for(float x = 0.006; x < 0.01; x+=mesh_geometry.sub_scale){
//     for(float y = 0.006; y < 0.01; y+=mesh_geometry.sub_scale){
//       for(float z = 0.006; z < 0.01; z+=mesh_geometry.sub_scale){
//         set_mesh_value_world_point(1000.0,x,y,z,potentials,mesh_geometry);
//         set_mesh_value_world_point(1,x,y,z,boundaries,mesh_geometry);
//       }
//     }
//   }
//
//
// //   set_mesh_value(1000.0,200,200,200,potentials,mesh_geometry);
// //   set_mesh_value(1,200,200,200,boundaries,mesh_geometry);
// // //
// // set_mesh_value_world_point(1,x,y,z,boundaries,mesh_geometry);
// //
//
//   relax_laplace_potentials(potentials, boundaries, mesh_geometry, 0.01,1);
//   printf("%f\n", get_mesh_value_world_point(0.007,0.007,0.007,potentials,mesh_geometry));
//
//   for(float x = 0.0; x < 0.01; x+=mesh_geometry.sub_scale){
//       printf("%f\n", get_mesh_value_world_point(x,0.005,0.005,potentials,mesh_geometry));
//   }
//
//   DOUBLES_EQUAL(0.58823, get_mesh_value(10,10,10,potentials,mesh_geometry), 1e-2);
// }
//
//



TEST(laplace_tests,fast_laplace_convergence_1){

  //
  // float mesh_bounds[6] = {0,0.1,0,0.1,0,0.1};
  //
  // root_mesh_geometry mesh_geometry(mesh_bounds, 0.003, 0.00005);
  //
  // std::vector<std::vector<float>> potentials;
  // std::vector<std::vector<int>> boundaries;
  //
  // potentials.resize(mesh_geometry.root_size);
  // boundaries.resize(mesh_geometry.root_size);
  //
  // float mesh_active_bounds[6] = {0,0.01,0,0.01,0,0.01};
  //
  // enable_mesh_region(potentials,mesh_active_bounds,mesh_geometry);
  // enable_mesh_region(boundaries,mesh_active_bounds,mesh_geometry);
  //
  // for(float x = 0.006; x < 0.01; x+=mesh_geometry.sub_scale){
  //   for(float y = 0.006; y < 0.01; y+=mesh_geometry.sub_scale){
  //     for(float z = 0.006; z < 0.01; z+=mesh_geometry.sub_scale){
  //       set_mesh_value_world_point(1000.0,x,y,z,potentials,mesh_geometry);
  //       set_mesh_value_world_point(1,x,y,z,boundaries,mesh_geometry);
  //     }
  //   }
  // }


//   set_mesh_value(1000.0,200,200,200,potentials,mesh_geometry);
//   set_mesh_value(1,200,200,200,boundaries,mesh_geometry);
// //
// set_mesh_value_world_point(1,x,y,z,boundaries,mesh_geometry);
//

  fast_relax_laplace_potentials(1);
  // printf("%f\n", get_mesh_value_world_point(0.007,0.007,0.007,potentials,mesh_geometry));
  //
  // for(float x = 0.0; x < 0.01; x+=mesh_geometry.sub_scale){
  //     printf("%f\n", get_mesh_value_world_point(x,0.005,0.005,potentials,mesh_geometry));
  // }
  //
  // DOUBLES_EQUAL(0.58823, get_mesh_value(10,10,10,potentials,mesh_geometry), 1e-2);
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
