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

//
// TEST(idx_tests, geometry_construct_test)
// {
//   float mesh_bounds[6] = {0.1,0.3,0.05,0.1,0,0.5};
//
//   root_mesh_geometry test(mesh_bounds, 0.003, 0.000051);
//
//   CHECK_EQUAL(67, test.root_x_len);
//   CHECK_EQUAL(17, test.root_y_len);
//   CHECK_EQUAL(167, test.root_z_len);
//
//   DOUBLES_EQUAL(0.003, test.root_scale,1e-6);
//
//   DOUBLES_EQUAL(0.1, test.x_min_bound,1e-6);
//   DOUBLES_EQUAL(0.301, test.x_max_bound,1e-6);
//   DOUBLES_EQUAL(0.05, test.y_min_bound,1e-6);
//   DOUBLES_EQUAL(0.101, test.y_max_bound,1e-6);
//   DOUBLES_EQUAL(0.0, test.z_min_bound,1e-6);
//   DOUBLES_EQUAL(0.501, test.z_max_bound,1e-6);
//
//   CHECK_EQUAL(58, test.sub_len);
//   DOUBLES_EQUAL(0.0000517, test.sub_scale,1e-6);
//
// }
//
//
// TEST(idx_tests, vector_idx_test)
// {
//   float mesh_bounds[6] = {0.1,0.3,0.05,0.1,0,0.5};
//
//   root_mesh_geometry test(mesh_bounds, 0.003, 0.000051);
//
//
// }
//
//
TEST_GROUP(laplace_tests){};

TEST(laplace_tests,coarsen){

  float mesh_bounds[6] = {0,0.1,0,0.1,0,0.1};

  root_mesh_geometry mesh_geometry(mesh_bounds, 0.003);

  std::vector<std::vector<float>> potentials;
  std::vector<std::vector<int>> boundaries;

  float mesh_active_bounds[6] = {0,0.003,0,0.003,0,0.003};

  enable_mesh_region(potentials,boundaries,mesh_active_bounds,mesh_geometry,20);

  for(float sub = 0; sub < 20; sub++){
    potentials[0][sub] = sub;
  }

  std::vector<std::vector<float>> coarsened_potentials;

  coarsen_mesh(potentials,coarsened_potentials,mesh_geometry,2);

    // for(uint32_t root = 0; root < 20; root++){
  // for(uint32_t sub = 0; sub < 10; sub++){
  //   printf("%f\n",coarsened_potentials[0][sub]);
  // }
    // }

  CHECK_EQUAL(4, potentials[0][4]);

  std::vector<std::vector<float>> decoarsened_potentials;

  decoarsen_mesh(coarsened_potentials,decoarsened_potentials,mesh_geometry,2);

  // for(uint32_t sub = 0; sub < 10; sub++){
  //   printf("%f\n",decoarsened_potentials[0][sub]);
  // }

}


//
// TEST(laplace_tests,get_mesh){
//   float mesh_bounds[6] = {0,0.1,0,0.1,0,0.1};
//
//   root_mesh_geometry mesh_geometry(mesh_bounds, 0.003);
//
//   std::vector<std::vector<float>> potentials;
//   std::vector<std::vector<int>> boundaries;
//
//   potentials.resize(mesh_geometry.root_size);
//
//   float mesh_active_bounds[6] = {0,0.02,0,0.01,0,0.01};
//
//   enable_mesh_region(potentials,mesh_active_bounds,mesh_geometry);
//
//   set_mesh_value(1000.0,150,10,10,potentials,mesh_geometry);
//
//   CHECK_EQUAL(1000.0,get_mesh_value(150,10,10,potentials,mesh_geometry));
//
//
// }


TEST(laplace_tests,activate_submesh){

  float mesh_bounds[6] = {0,0.1,0,0.1,0,0.1};

  root_mesh_geometry mesh_geometry(mesh_bounds, 0.003);

  std::vector<std::vector<float>> potentials;
  std::vector<std::vector<int>> boundaries;

  potentials.resize(mesh_geometry.root_size);

  float mesh_active_bounds[6] = {0.003,0.006,0,0.003,0,0.003};



  enable_mesh_region(potentials,boundaries,mesh_active_bounds,mesh_geometry,60);





  CHECK_EQUAL(0,potentials[0].size());
  CHECK_EQUAL(60*60*60,potentials[1].size());
  CHECK_EQUAL(0,potentials[2].size());

  // DOUBLES_EQUAL(0.58823, potentials[10][i_idx(3,1,1,mesh_geometry)], 1e-2);
}



TEST(laplace_tests,fast_laplace_convergence_1_big){


  float mesh_bounds[6] = {0,0.1,0,0.1,0,0.1};

  root_mesh_geometry mesh_geometry(mesh_bounds, 0.003);

  std::vector<std::vector<float>> potentials;
  std::vector<std::vector<int>> boundaries;

  float mesh_active_bounds[6] = {0,0.01,0,0.01,0,0.1};

  enable_mesh_region(potentials,boundaries,mesh_active_bounds,mesh_geometry,60);

  for(uint32_t root = 0; root < 200; root++){
    for(uint32_t sub = 0; sub < potentials[root].size(); sub++){
      potentials[root][sub] = 1000.0;
      boundaries[root][sub] = 1;
    }
  }

  fast_relax_laplace_potentials(potentials, boundaries, mesh_geometry, 0.01, 1, 1);
  //takes 47 seconds ATM
  // to_csv(potentials,mesh_geometry);


  // printf("%f\n", get_mesh_value_world_point(0.007,0.007,0.007,potentials,mesh_geometry));
  //
  // for(float x = 0.0; x < 0.01; x+=mesh_geometry.sub_scale){
  //     printf("%f\n", get_mesh_value_world_point(x,0.005,0.005,potentials,mesh_geometry));
  // }
  //
  // DOUBLES_EQUAL(0.58823, get_mesh_value(10,10,10,potentials,mesh_geometry), 1e-2);
}






TEST(laplace_tests,fast_laplace_convergence_1_point_tests){


  float mesh_bounds[6] = {0,0.1,0,0.1,0,0.1};

  root_mesh_geometry mesh_geometry(mesh_bounds, 0.003);

  std::vector<std::vector<float>> potentials;
  std::vector<std::vector<int>> boundaries;

  float mesh_active_bounds[6] = {0,0.01,0,0.01,0,0.01};

  enable_mesh_region(potentials,boundaries,mesh_active_bounds,mesh_geometry,10);

  for(uint32_t root = 0; root < 1; root++){
    for(uint32_t sub = 0; sub < potentials[root].size(); sub++){
      potentials[root][sub] = 1000.0;
      boundaries[root][sub] = 1;
    }
  }

  int fine_sub_idx = (20*20*(19)) + (20*19) + 19;
  potentials[0][fine_sub_idx] = 1000.0;
  boundaries[0][fine_sub_idx] = 1;


  // to_csv(potentials,mesh_geometry);

  fast_relax_laplace_potentials(potentials, boundaries, mesh_geometry, 0.01, 1, 1);
  //takes 47 seconds ATM

  for(int r_x = 0; r_x < mesh_geometry.root_x_len; r_x++){
    for(int r_y = 0; r_y < mesh_geometry.root_y_len; r_y++){
      for(int r_z = 0; r_z < mesh_geometry.root_z_len; r_z++){
        if((int)potentials[idx(r_x,r_y,r_z,mesh_geometry.root_x_len,mesh_geometry.root_y_len)].size()){
          printf("x%i,y%i,z%i,s%i,%f\n",r_x,r_y,r_z,(int)potentials[idx(r_x,r_y,r_z,mesh_geometry.root_x_len,mesh_geometry.root_y_len)].size(),potentials[idx(r_x,r_y,r_z,mesh_geometry.root_x_len,mesh_geometry.root_y_len)][0]);
        }
      }
    }
  }


  to_csv(potentials,mesh_geometry);


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
