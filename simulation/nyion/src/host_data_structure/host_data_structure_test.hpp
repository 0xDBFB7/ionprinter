#include "nyion.hpp"

#include "gtest/gtest.h"

#ifndef DATA_STRUCTURE_TEST_H
#define DATA_STRUCTURE_TEST_H



TEST(xyz_traverse, ghost_traverse_test){
  int mesh_sizes[MAX_DEPTH];
  std::fill(mesh_sizes, mesh_sizes + MAX_DEPTH, 3);

  traverse_state state;
  init_state(state, mesh_sizes);
  EXPECT_EQ(0,state.x);
  EXPECT_EQ(0,state.y);
  EXPECT_EQ(0,state.z);
  xyz_traverse(state,mesh_sizes,0);
  EXPECT_EQ(1,state.x);
  EXPECT_EQ(0,state.y);
  EXPECT_EQ(0,state.z);
  xyz_traverse(state,mesh_sizes,0);
  EXPECT_EQ(2,state.x);
  EXPECT_EQ(0,state.y);
  EXPECT_EQ(0,state.z);
  xyz_traverse(state,mesh_sizes,0);
  EXPECT_EQ(0,state.x);
  EXPECT_EQ(1,state.y);
  EXPECT_EQ(0,state.z);
}


TEST(xyz_traverse, non_ghost_traverse_test){
  int mesh_sizes[MAX_DEPTH];
  std::fill(mesh_sizes, mesh_sizes + MAX_DEPTH, 4);

  traverse_state state;
  init_state(state, mesh_sizes);

  xyz_traverse(state,mesh_sizes, 1);
  EXPECT_EQ(1,state.x);
  EXPECT_EQ(1,state.y);
  EXPECT_EQ(1,state.z);
  xyz_traverse(state,mesh_sizes, 1);
  EXPECT_EQ(2,state.x);
  EXPECT_EQ(1,state.y);
  EXPECT_EQ(1,state.z);
}

TEST(init_state, scale_tests){
  int mesh_sizes[MAX_DEPTH];
  mesh_sizes[0] = 3;
  mesh_sizes[1] = 5;
  mesh_sizes[2] = 5;
  traverse_state state;
  init_state(state, mesh_sizes);
  ASSERT_NEAR(ROOT_WORLD_SCALE*(1/(3.0-2)), state.world_scale[0], 1e-5); //-2 because ghosts
  ASSERT_NEAR(ROOT_WORLD_SCALE*(1/(3.0-2))*(1/(5.0-2)), state.world_scale[1], 1e-5);
  ASSERT_NEAR(ROOT_WORLD_SCALE*(1/(3.0-2))*(1/(5.0-2))*(1/(5.0-2)),state.world_scale[2], 1e-5);
}

TEST(cell_world_lookup, cell_world_lookup_test_3){
  float x,y,z;
  traverse_state state;
  int mesh_sizes[MAX_DEPTH] = {3, 3, 3};
  init_state(state, mesh_sizes);

  cell_world_lookup(state, x, y, z);

  ASSERT_NEAR(-ROOT_WORLD_SCALE, x, 1e-5);
  ASSERT_NEAR(-ROOT_WORLD_SCALE, y, 1e-5);
  ASSERT_NEAR(-ROOT_WORLD_SCALE, z, 1e-5);

  xyz_traverse(state, mesh_sizes, 0);

}


// TEST(cell_world_lookup, cell_world_lookup_test_5){
//   float x,y,z;
//   traverse_state state;
//   int mesh_sizes[MAX_DEPTH];
//   std::fill(mesh_sizes, mesh_sizes + MAX_DEPTH, 5);
//   init_state(state, mesh_sizes);
//
//   state.current_depth = 2;
//   state.x_queue[0] = 1;
//   state.x_queue[1] = 1;
//   state.x_queue[2] = 1;
//   cell_world_lookup(state, x, y, z);
//
//   ASSERT_NEAR(, x, 1e-5);
//   ASSERT_NEAR(-ROOT_WORLD_SCALE, y, 1e-5);
//   ASSERT_NEAR(-ROOT_WORLD_SCALE, z, 1e-5);
//
// }



class breadth_first_test : public ::testing::Test {
 protected:
  void SetUp() override {
    std::fill(mesh_sizes, mesh_sizes + MAX_DEPTH, test_mesh_size);

    test = new int[MESH_BUFFER_SIZE];
    order = new int[MESH_BUFFER_SIZE];
    refined_indices = new int[MESH_BUFFER_SIZE];

    std::fill(test, test + MESH_BUFFER_SIZE, 100);
    std::fill(order, order + MESH_BUFFER_SIZE, 100);
    std::fill(refined_indices, refined_indices + MESH_BUFFER_SIZE, 0);
  }

  void TearDown() override {
    delete [] test;
    delete [] order;
    delete [] refined_indices;
  }

  int * test;
  int * order;
  int * refined_indices;
  int mesh_sizes[MAX_DEPTH];
  int test_mesh_size = 2;

};

// TEST(physics_mesh_test, create_test){
//     int mesh_sizes[MAX_DEPTH] = {3, 3, 3};
//     physics_mesh test_mesh(mesh_sizes);
//     test_mesh.temperature[10] = 1;
// }
//
// TEST_F(breadth_first_test, traverse_test_2x2_2x2){
//   int heap_end = mesh_sizes[0]*mesh_sizes[0]*mesh_sizes[0];
//   refined_indices[2] = heap_end;//create refinement in the center-middle
//
//   traverse_state state;
//   for(init_state(state, mesh_sizes); breadth_first(state, refined_indices, MAX_DEPTH, 0, mesh_sizes); xyz_traverse(state, mesh_sizes, 0)){
//     test[state.current_indice] = state.current_indice-state.block_beginning_indice;
//   }
//
//   pretty_print_named_array(test, 0, 2*(test_mesh_size*test_mesh_size*test_mesh_size));
//
//   for(int i = 0; i < pow(test_mesh_size,3); i++){ EXPECT_EQ(i,test[i]); }; //root region
//   for(int i = pow(test_mesh_size,3); i < pow(test_mesh_size,3)*2; i++){ EXPECT_EQ(i-pow(test_mesh_size,3),test[i]); }; //level 1
//
//   std::fill(test, test + MESH_BUFFER_SIZE, 100);
//   std::fill(order, order + MESH_BUFFER_SIZE, 100);
//
//   pretty_print_named_array(test, 0, 2*(test_mesh_size*test_mesh_size*test_mesh_size));
//
// }
//
// TEST_F(breadth_first_test, traverse_test_3x3_3x3){
//
// }

//
// TEST_GROUP(data_structure_benchmark){
// };
//
//
// TEST(data_structure_mesh, traverse_benchmark){
//   int test_mesh_size = 10;
//
//   int mesh_sizes[MAX_DEPTH];
//   std::fill(mesh_sizes, mesh_sizes + MAX_DEPTH, test_mesh_size);
//
//
//   int * test = new int[MESH_BUFFER_SIZE];
//   int * refined_indices = new int[MESH_BUFFER_SIZE];
//   std::fill(test, test + MESH_BUFFER_SIZE, 0);
//   std::fill(refined_indices, refined_indices + MESH_BUFFER_SIZE, 0);
//
//
//   int heap_end = mesh_sizes[0]*mesh_sizes[0]*mesh_sizes[0];
//   for(int i = 0; i < pow(test_mesh_size,3); i++){
//     refined_indices[i] = heap_end;//center-middle
//     heap_end += pow(test_mesh_size,3);
//   }
//
//   traverse_state state;
//
//   auto start = std::chrono::high_resolution_clock::now();
//
//   for(init_state(state, mesh_sizes); breadth_first(state, refined_indices, MAX_DEPTH, 0, mesh_sizes); xyz_traverse(state, mesh_sizes, 0)){
//     test[state.current_indice] = state.current_indice-state.block_beginning_indice;
//   }
//
//   auto stop = std::chrono::high_resolution_clock::now();
//   auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
//   std::cout << "Time taken by function: " << duration.count() << " microseconds\n";
//
//
//   delete [] test;
//   delete [] refined_indices;
// }
//
//
#endif
