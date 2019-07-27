#include "multiphysics.hpp"
#include "visualize.hpp"
#include "stencils.hpp"
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"



TEST_GROUP(idx_tests){};


TEST(idx_tests, geometry_construct_test)
{

  double mesh_bounds[6] = {0.1,0.3,0.05,0.1,0,0.5};

  root_mesh_geometry test(mesh_bounds, 0.003);

  CHECK_EQUAL(67, test.root_x_len);
  CHECK_EQUAL(17, test.root_y_len);
  CHECK_EQUAL(167, test.root_z_len);
  CHECK_EQUAL(190213,test.root_size);

  DOUBLES_EQUAL(0.1,test.x_min_bound,1e-6);
  DOUBLES_EQUAL(0.301,test.x_max_bound,1e-6);
  DOUBLES_EQUAL(0.05,test.y_min_bound,1e-6);
  DOUBLES_EQUAL(0.101,test.y_max_bound,1e-6);
  DOUBLES_EQUAL(0.0,test.z_min_bound,1e-6);
  DOUBLES_EQUAL(0.501,test.z_max_bound,1e-6);

}


TEST_GROUP(laplace_tests){};

TEST(laplace_tests,submesh_side){
    std::vector<std::vector<double>> potentials;

    potentials.resize(10);
    potentials[0].resize(idx(9,9,9,10,10)+1);

    CHECK_EQUAL(10, submesh_side_length(potentials[0]));
    CHECK_EQUAL(0, submesh_side_length(potentials[1]));


    /* -----------------------------------------------------------------------------
    Exhaustively test.
    ----------------------------------------------------------------------------- */

    for(int i = 1; i < 300; i++){
      potentials[0].resize(i*i*i);

      CHECK_EQUAL(i, submesh_side_length(potentials[0]));
    }
}



TEST(laplace_tests,activate_submesh_even){

  double mesh_bounds[6] = {0,0.1,0,0.1,0,0.1};

  root_mesh_geometry mesh_geometry(mesh_bounds, 0.003);

  std::vector<std::vector<double>> potentials;

  double mesh_active_bounds[6] = {0.003,0.006,0,0.003,0,0.003};

  enable_mesh_region(potentials,mesh_active_bounds,mesh_geometry,60);

  CHECK_EQUAL(0,potentials[root_idx(0,0,0,mesh_geometry)].size());
  CHECK_EQUAL(60*60*60,potentials[root_idx(1,0,0,mesh_geometry)].size());
  CHECK_EQUAL(0,potentials[root_idx(0,0,0,mesh_geometry)].size());
  CHECK_EQUAL(0,potentials[root_idx(0,1,0,mesh_geometry)].size());
}

TEST(laplace_tests,activate_submesh_round){

  double mesh_bounds[6] = {0,0.1,0,0.1,0,0.1};

  root_mesh_geometry mesh_geometry(mesh_bounds, 0.003);

  std::vector<std::vector<double>> potentials;

  double mesh_active_bounds[6] = {0.0035,0.004,0,0.0035,0,0.003};

  enable_mesh_region(potentials,mesh_active_bounds,mesh_geometry,60);

  CHECK_EQUAL(0,potentials[root_idx(0,0,0,mesh_geometry)].size());
  CHECK_EQUAL(60*60*60,potentials[root_idx(1,0,0,mesh_geometry)].size());
  CHECK_EQUAL(0,potentials[root_idx(0,0,0,mesh_geometry)].size());
  CHECK_EQUAL(0,potentials[root_idx(0,1,0,mesh_geometry)].size());
}

//
// TEST(laplace_tests,relative_indexing){
//
//   double mesh_bounds[6] = {0,0.009,0,0.009,0,0.009};
//
//   root_mesh_geometry mesh_geometry(mesh_bounds, 0.003);
//
//   std::vector<std::vector<double>> potentials;
//
//   double mesh_active_bounds[6] = {0.000,0.009,0,0.009,0,0.009}; // a 3x3x3 mesh
//
//   enable_mesh_region(potentials,mesh_active_bounds,mesh_geometry,10);
//
//   //we'll make the upper-right-corner tests go across a submesh length boundary
//   potentials[root_idx(1,1,2,mesh_geometry)].resize(5*5*5);
//
//   int number_of_points = 9;
//   //format:
//   //relative root, relative sub, relative delta, actual root, actual sub, valid
//   int test_points[][16] = {
//             {1,1,1, 0,0,0, 0,0,0, 1,1,1, 0,0,0, 1}, //no movement
//             {1,1,1, 0,0,0, 0,0,-1, 1,1,0, 0,0,9, 1}, //down in z across boundary
//             {1,1,1, 0,0,0, 0,0,1, 1,1,1, 0,0,1, 1}, // up in z
//             {1,1,1, 0,0,0, 0,-1,0, 1,0,1, 0,9,0, 1}, // -y, across boundary
//             {1,1,1, 0,0,0, 0,1,0, 1,1,1, 0,1,0, 1}, // +y
//             {1,1,1, 0,0,0, -1,0,0, 0,1,1, 9,0,0, 1}, // -x, across boundary
//             {1,1,1, 0,0,0, 1,0,0, 1,1,1, 1,0,0, 1}, // +x
//
//             {1,1,1, 9,9,9, 0,0,1, 1,1,2, 4,4,0, 1}, //top corner, up in z, across size boundary
//
//             {0,0,0, 0,0,0, 0,0,-1, 1,1,2, 4,4,0, 0} //invalid
//             };
//
//   bool valid = false;
//
//
//   for(int i = 0; i < number_of_points; i++){
//     int root = root_idx(test_points[i][9],test_points[i][10],test_points[i][11],mesh_geometry);
//     int side_len = submesh_side_length(potentials[root]);
//     potentials[root][idx(test_points[i][12],test_points[i][13],test_points[i][14],side_len,side_len)] = i+1;
//
//
//     double value = relative_mesh_value(potentials,
//                       root_idx(test_points[i][0],test_points[i][1],test_points[i][2],mesh_geometry),
//                             test_points[i][3],test_points[i][4],test_points[i][5],
//                             test_points[i][6],test_points[i][7],test_points[i][8], mesh_geometry, valid);
//
//     CHECK_EQUAL((i+1)*valid, value);
//
//     CHECK_EQUAL((bool) test_points[i][15], valid);
//   }
//
// }


TEST(laplace_tests,relative_indexing){

  double mesh_bounds[6] = {0,0.009,0,0.009,0,0.009};

  root_mesh_geometry mesh_geometry(mesh_bounds, 0.003);

  std::vector<std::vector<double>> potentials;

  double mesh_active_bounds[6] = {0.000,0.009,0,0.009,0,0.009}; // a 3x3x3 mesh

  enable_mesh_region(potentials,mesh_active_bounds,mesh_geometry,10);

  //we'll make the upper-right-corner tests go across a submesh length boundary
  potentials[root_idx(1,1,2,mesh_geometry)].resize(5*5*5);

  bool valid = false;
  double value = 0;

  /* -----------------------------------------------------------------------------
  +z init
  ----------------------------------------------------------------------------- */
  for(int i = 0; i < mesh_geometry.root_size; i++){
    std::fill(potentials[i].begin(), potentials[i].end(), 1);
  }
  /* -----------------------------------------------------------------------------
  A valid point
  ----------------------------------------------------------------------------- */
  potentials[root_idx(1,1,2,mesh_geometry)][idx(4,4,0,5,5)] = 10;
  value = value_plus_z(potentials, 0, 0, 0,
                                  0, 0, 0,
                                  10, 3, 3, 3, valid);
  CHECK_EQUAL(valid,true);
  DOUBLES_EQUAL(1,value,1e-5);
  /* -----------------------------------------------------------------------------
  An invalid point
  ----------------------------------------------------------------------------- */
  value = value_plus_z(potentials, 2, 2, 2,
                                  9, 9, 9,
                                  10, 3, 3, 3, valid);
  CHECK_EQUAL(valid,false);
  DOUBLES_EQUAL(0,value,1e-5);
  /* -----------------------------------------------------------------------------
  A point that crosses a submesh length boundary
  ----------------------------------------------------------------------------- */
  potentials[root_idx(1,1,2,mesh_geometry)][idx(4,4,0,5,5)] = 10;
  printf("%i\n",root_idx(1,1,2,mesh_geometry));
  value = value_plus_z(potentials, 1, 1, 1,
                                  9, 9, 9,
                                  10, 3, 3, 3, valid);
  CHECK_EQUAL(valid,true);
  DOUBLES_EQUAL(10,value,1e-5);


  /* -----------------------------------------------------------------------------
  A point that crosses a submesh length boundary
  ----------------------------------------------------------------------------- */

}



TEST(laplace_tests,relative_timing){

  double mesh_bounds[6] = {0,0.009,0,0.009,0,0.009};

  root_mesh_geometry mesh_geometry(mesh_bounds, 0.003);

  std::vector<std::vector<double>> potentials;

  double mesh_active_bounds[6] = {0.000,0.009,0,0.009,0,0.009}; // a 3x3x3 mesh

  enable_mesh_region(potentials,mesh_active_bounds,mesh_geometry,10);

  auto t1 = std::chrono::high_resolution_clock::now();
  bool valid = false;
  for(int i = 0; i < 100; i++){
    potentials[0][0] = i+1;

    relative_mesh_value(potentials,
                      0,0,0,0,0,0,0, mesh_geometry, valid);
  }

  auto t2 = std::chrono::high_resolution_clock::now();
  std::cout << "relative mesh lookup takes " << (std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count())/(double) 100.0 << " us" << "\n";

}




TEST(laplace_tests,coarsen){
  /* -----------------------------------------------------------------------------
  Simple exhaustive test of mesh coarsening
  ----------------------------------------------------------------------------- */

  double mesh_bounds[6] = {0,0.1,0,0.1,0,0.1};
  root_mesh_geometry mesh_geometry(mesh_bounds, 0.003);

  std::vector<std::vector<double>> potentials;

  double mesh_active_bounds[6] = {0,0.006,0,0.006,0,0.006};

  enable_mesh_region(potentials,mesh_active_bounds,mesh_geometry,10);

  for(int root = 0; root < mesh_geometry.root_size; root++){
    int side_len = submesh_side_length(potentials[root]);
    for(int x = 0; x < side_len; x++){
      for(int y = 0; y < side_len; y++){
        for(int z = 0; z < side_len; z++){
          potentials[root][idx(x,y,z,side_len,side_len)] = (root*100000)+idx(x,y,z,side_len,side_len);
        }
      }
    }
  }

  std::vector<std::vector<double>> coarsened_potentials;

  int test_divisor = 5;

  coarsen_mesh(potentials,coarsened_potentials,mesh_geometry,test_divisor);

  for(int root = 0; root < mesh_geometry.root_size; root++){
    int side_len = submesh_side_length(potentials[root]);;
    for(int x = 0; x < side_len/test_divisor; x++){
      for(int y = 0; y < side_len/test_divisor; y++){
        for(int z = 0; z < side_len/test_divisor; z++){
          CHECK_EQUAL((root*100000)+idx(x*test_divisor,y*test_divisor,z*test_divisor,side_len,side_len),
                                    coarsened_potentials[root][idx(x,y,z,side_len/test_divisor,side_len/test_divisor)]);
        }
      }
    }
  }


}


TEST(laplace_tests,refine){

  double mesh_bounds[6] = {0,0.1,0,0.1,0,0.1};
  root_mesh_geometry mesh_geometry(mesh_bounds, 0.003);

  std::vector<std::vector<double>> potentials;

  double mesh_active_bounds[6] = {0,0.006,0,0.006,0,0.006};

  for(int test_divisor = 2; test_divisor < 4; test_divisor++){

    for(int input_mesh_len = 2; input_mesh_len < 60; input_mesh_len+=30){

      enable_mesh_region(potentials,mesh_active_bounds,mesh_geometry,input_mesh_len);

      /* -----------------------------------------------------------------------------
      Write beyond a single submesh to test behavior near submesh edges.
      ----------------------------------------------------------------------------- */
      for(int i = 0; i < 2; i++){
        for(int x = 0; x < input_mesh_len; x++){
          potentials[root_idx(i,0,0,mesh_geometry)][idx(x,0,0,input_mesh_len,input_mesh_len)] = x + (input_mesh_len*i);
        }
      }

      for(int i = 0; i < 2; i++){
        for(int y = 0; y < input_mesh_len; y++){
          potentials[root_idx(0,i,0,mesh_geometry)][idx(0,y,0,input_mesh_len,input_mesh_len)] = y + (input_mesh_len*i);
        }
      }

      for(int i = 0; i < 2; i++){
        for(int z = 0; z < input_mesh_len; z++){
          potentials[root_idx(0,0,i,mesh_geometry)][idx(0,0,z,input_mesh_len,input_mesh_len)] = z + (input_mesh_len*i);
        }
      }

      std::vector<std::vector<double>> refined_potentials;


      decoarsen_mesh(potentials,refined_potentials,mesh_geometry,test_divisor);

      for(int x = 0; x < (input_mesh_len*test_divisor); x++){
        DOUBLES_EQUAL((input_mesh_len/ (double) (input_mesh_len*test_divisor))*x,refined_potentials[0][x], 1e-4);
      }

      for(int y = 0; y < (input_mesh_len*test_divisor)-test_divisor; y++){
        DOUBLES_EQUAL((input_mesh_len/ (double) (input_mesh_len*test_divisor))*y,
                          refined_potentials[0][idx(0,y,0,input_mesh_len*test_divisor,input_mesh_len*test_divisor)],1e-4);
      }

      for(int z = 0; z < (input_mesh_len*test_divisor)-test_divisor; z++){
        DOUBLES_EQUAL((input_mesh_len/ (double) (input_mesh_len*test_divisor))*z,
                          refined_potentials[0][idx(0,z,0,input_mesh_len*test_divisor,input_mesh_len*test_divisor)],1e-4);
      }
    }

  }

}




//
//
// TEST(laplace_tests,fast_laplace_convergence_1_big){
//
//
//   double mesh_bounds[6] = {0,0.1,0,0.1,0,0.1};
//
//   root_mesh_geometry mesh_geometry(mesh_bounds, 0.003);
//
//   std::vector<std::vector<double>> potentials;
//   std::vector<std::vector<int>> boundaries;
//
//   double mesh_active_bounds[6] = {0,0.01,0,0.01,0,0.01};
//
//   enable_mesh_region(potentials,boundaries,mesh_active_bounds,mesh_geometry,30);
//
//   // for(uint32_t root = 0; root < 1; root++){
//     for(uint32_t sub = 0; sub < potentials[idx(1,1,1,mesh_geometry.root_x_len,mesh_geometry.root_y_len)].size()/2; sub++){
//       potentials[idx(1,1,1,mesh_geometry.root_x_len,mesh_geometry.root_y_len)][sub] = 1000.0;
//       boundaries[idx(1,1,1,mesh_geometry.root_x_len,mesh_geometry.root_y_len)][sub] = 1;
//     }
//   // }
//
//   fast_relax_laplace_potentials(potentials, boundaries, mesh_geometry, 1, 1, 1);
//   //takes 47 seconds ATM
//   // to_csv(potentials,mesh_geometry);
//
//
//   // printf("%f\n", get_mesh_value_world_point(0.007,0.007,0.007,potentials,mesh_geometry));
//   //
//   // for(double x = 0.0; x < 0.01; x+=mesh_geometry.sub_scale){
//   //     printf("%f\n", get_mesh_value_world_point(x,0.005,0.005,potentials,mesh_geometry));
//   // }
//   //
//   // DOUBLES_EQUAL(0.58823, get_mesh_value(10,10,10,potentials,mesh_geometry), 1e-2);
// }
//

TEST(laplace_tests,gauss_seidel_point_tests){
  double mesh_bounds[6] = {0,0.1,0,0.1,0,0.1};
  root_mesh_geometry mesh_geometry(mesh_bounds, 0.003);

  std::vector<std::vector<double>> potentials;
  std::vector<std::vector<int>> boundaries;

  double mesh_active_bounds[6] = {0,0.006,0,0.006,0,0.006};

  enable_mesh_region(potentials,mesh_active_bounds,mesh_geometry,10);
  enable_mesh_region(boundaries,mesh_active_bounds,mesh_geometry,10);

  potentials[0][idx(4,0,0,10,10)] = 1000.0;
  boundaries[0][idx(4,0,0,10,10)] = 10;

  std::vector<std::vector<double>> residuals;

  residuals = gauss_seidel(potentials, boundaries, mesh_geometry, 50, 1, 0);


  // for(int x = 0; x < 10; x++){
  //   printf("%f\n",residuals[0][x]);
  //   // DOUBLES_EQUAL((input_mesh_len/ (double) (input_mesh_len*test_divisor))*x,refined_potentials[0][x], 1e-4);
  // }

}
//
// TEST(laplace_tests,v_cycle_tests){
//   double mesh_bounds[6] = {0,0.05,0,0.05,0,0.3};
//   root_mesh_geometry mesh_geometry(mesh_bounds, 0.003);
//
//   std::vector<std::vector<double>> potentials;
//   std::vector<std::vector<int>> boundaries;
//
//   double mesh_active_bounds[6] = {0,0.006,0,0.006,0,0.02};
//
//   enable_mesh_region(potentials,mesh_active_bounds,mesh_geometry,20);
//   enable_mesh_region(boundaries,mesh_active_bounds,mesh_geometry,20);
//
//   potentials[0][idx(4,0,0,10,10)] = 1000.0;
//   boundaries[0][idx(4,0,0,10,10)] = 10;
//
//   while(1){
//     printf("\n\n");
//     v_cycle(potentials,boundaries,mesh_geometry,1,0);
//     to_csv(potentials,mesh_geometry);
//   }
//
//   // to_csv(potentials,mesh_geometry);
// }
//

// TEST(laplace_tests,opengl_simple_boundary){
//     double mesh_bounds[6] = {0,0.05,0,0.05,0,0.05};
//     root_mesh_geometry mesh_geometry(mesh_bounds, 0.003);
//
//     std::vector<std::vector<double>> potentials;
//     std::vector<std::vector<int>> boundaries;
//
//     double mesh_active_bounds[6] = {0,0.05,0,0.06,0,0.05};
//
//     enable_mesh_region(potentials,mesh_active_bounds,mesh_geometry,2);
//     enable_mesh_region(boundaries,mesh_active_bounds,mesh_geometry,2);
//
//     potentials[0][1] = -1000;
//     boundaries[0][1] = 1;
//     potentials[10][1] = 1000;
//     boundaries[10][1] = 1;
//     initialize_opengl(mesh_geometry);
//     std::vector<double> test_graph = {10,-10,0.3};
//
//     while(true){
//
//       opengl_switch_to_graph_window();
//       test_graph.push_back(10);
//       opengl_graph_1d_vector(test_graph,"Test",0);
//
//       opengl_2d_mode();
//
//       opengl_switch_to_mesh_window();
//
//       opengl_3d_mode();
//
//       opengl_apply_camera_rotation();
//
//       opengl_draw_axis_cross();
//
//       draw_geometry_outline(mesh_geometry);
//       gauss_seidel(potentials, boundaries, mesh_geometry, 5, 1, 0);
//       draw_mesh(potentials,mesh_geometry);
//
//       opengl_2d_mode();
//
//
//       update_screen();
//
//       opengl_switch_to_graph_window();
//       opengl_clear_screen();
//
//       opengl_switch_to_mesh_window();
//       opengl_clear_screen();
//       // update_screen();
//        bool valid = false;
//       printf("%f",relative_mesh_value(potentials,idx(10,0,16,17,17),16,16,16,0,0,-1,mesh_geometry,valid));
//
//     }
//
//
// }


  //
//
//
// TEST(laplace_tests,fast_laplace_convergence_1_point_tests){
//
//
//   double mesh_bounds[6] = {0,0.1,0,0.1,0,0.1};
//
//   root_mesh_geometry mesh_geometry(mesh_bounds, 0.003);
//
//   std::vector<std::vector<double>> potentials;
//   std::vector<std::vector<int>> boundaries;
//
//   double mesh_active_bounds[6] = {0,0.01,0,0.01,0,0.01};
//
//   enable_mesh_region(potentials,boundaries,mesh_active_bounds,mesh_geometry,10);
//
//   double mesh_active_bounds_2[6] = {0,0.003,0,0.003,0,0.003};
//
//   enable_mesh_region(potentials,boundaries,mesh_active_bounds_2,mesh_geometry,40);
//
//   for(uint32_t root = 0; root < 1; root++){
//     for(uint32_t sub = 0; sub < potentials[root].size(); sub++){
//       potentials[root][sub] = 1000.0;
//       boundaries[root][sub] = 1;
//     }
//   }
//
//   int fine_sub_idx = (20*20*(19)) + (20*19) + 19;
//   potentials[0][fine_sub_idx] = 1000.0;
//   boundaries[0][fine_sub_idx] = 1;
//
//
//   // to_csv(potentials,mesh_geometry);
//
//   fast_relax_laplace_potentials(potentials, boundaries, mesh_geometry, 0.01, 1, 1);
//   //takes 47 seconds ATM
//
//   for(int r_x = 0; r_x < mesh_geometry.root_x_len; r_x++){
//     for(int r_y = 0; r_y < mesh_geometry.root_y_len; r_y++){
//       for(int r_z = 0; r_z < mesh_geometry.root_z_len; r_z++){
//         if((int)potentials[idx(r_x,r_y,r_z,mesh_geometry.root_x_len,mesh_geometry.root_y_len)].size()){
//           printf("x%i,y%i,z%i,s%i,%f\n",r_x,r_y,r_z,(int)potentials[idx(r_x,r_y,r_z,mesh_geometry.root_x_len,mesh_geometry.root_y_len)].size(),potentials[idx(r_x,r_y,r_z,mesh_geometry.root_x_len,mesh_geometry.root_y_len)][0]);
//         }
//       }
//     }
//   }
//
//
//   // to_csv(potentials,mesh_geometry);
//
//
//   // printf("%f\n", get_mesh_value_world_point(0.007,0.007,0.007,potentials,mesh_geometry));
//   //
//   // for(double x = 0.0; x < 0.01; x+=mesh_geometry.sub_scale){
//   //     printf("%f\n", get_mesh_value_world_point(x,0.005,0.005,potentials,mesh_geometry));
//   // }
//   //
//   // DOUBLES_EQUAL(0.58823, get_mesh_value(10,10,10,potentials,mesh_geometry), 1e-2);
// }
//








//
// TEST(mesh_tests,mesh_import_test_2){
//
//   int mesh_geometry[3] = {1000,1,1};
//   double mesh_scale[3] = {0.001,0.001,0.001};
//   std::vector<bool> mesh_present(i_idx(mesh_geometry[X],mesh_geometry[Y],mesh_geometry[Z],mesh_geometry));
//   double bounds[6];
//   double translate[3] = {0.1,0,0};
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
