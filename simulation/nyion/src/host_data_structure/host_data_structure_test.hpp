#include "nyion.hpp"

#ifndef DATA_STRUCTURE_TEST_H
#define DATA_STRUCTURE_TEST_H



TEST_GROUP(data_structure_mesh){
};


TEST(data_structure_mesh, traverse_test){
  int mesh_sizes[MAX_DEPTH];
  std::fill(mesh_sizes, mesh_sizes + MAX_DEPTH, 3);

  traverse_state state;
  init_state(state, mesh_sizes);
  CHECK_EQUAL(0,state.x);
  CHECK_EQUAL(0,state.y);
  CHECK_EQUAL(0,state.z);
  xyz_traverse(state,mesh_sizes,0);
  CHECK_EQUAL(1,state.x);
  CHECK_EQUAL(0,state.y);
  CHECK_EQUAL(0,state.z);
  xyz_traverse(state,mesh_sizes,0);
  CHECK_EQUAL(2,state.x);
  CHECK_EQUAL(0,state.y);
  CHECK_EQUAL(0,state.z);
  xyz_traverse(state,mesh_sizes,0);
  CHECK_EQUAL(0,state.x);
  CHECK_EQUAL(1,state.y);
  CHECK_EQUAL(0,state.z);

  //with ghost ignore
  init_state(state, mesh_sizes);

  xyz_traverse(state,mesh_sizes, 1);
  CHECK_EQUAL(1,state.x);
  CHECK_EQUAL(1,state.y);
  CHECK_EQUAL(1,state.z);
}

TEST(data_structure_mesh, scale_tests){
  int mesh_sizes[MAX_DEPTH];
  mesh_sizes[0] = 3;
  mesh_sizes[1] = 5;
  mesh_sizes[2] = 3;
  // std::fill(mesh_sizes, mesh_sizes + MAX_DEPTH, 3);
  traverse_state state;
  init_state(state, mesh_sizes);
  DOUBLES_EQUAL(ROOT_WORLD_SCALE*(1/3.0), state.world_scale[0], 1e-5);
  DOUBLES_EQUAL(ROOT_WORLD_SCALE*(1/3.0)*(1/5.0), state.world_scale[1], 1e-5);
  DOUBLES_EQUAL(ROOT_WORLD_SCALE*(1/3.0)*(1/5.0)*(1/3.0),state.world_scale[2], 1e-5);
}

TEST_GROUP(exhaustive_traverse){
};


TEST(exhaustive_traverse, traverse_test_2x2_2x2){
  int test_mesh_size = 2;
  int mesh_sizes[MAX_DEPTH];
  std::fill(mesh_sizes, mesh_sizes + MAX_DEPTH, test_mesh_size);

  int * test = new int[MESH_BUFFER_SIZE];
  int * order = new int[MESH_BUFFER_SIZE];
  int * refined_indices = new int[MESH_BUFFER_SIZE];
  std::fill(test, test + MESH_BUFFER_SIZE, 100);
  std::fill(order, order + MESH_BUFFER_SIZE, 100);
  std::fill(refined_indices, refined_indices + MESH_BUFFER_SIZE, 0);

  int heap_end = mesh_sizes[0]*mesh_sizes[0]*mesh_sizes[0];
  refined_indices[2] = heap_end;//create refinement in the center-middle

  traverse_state state;
  for(init_state(state, mesh_sizes); breadth_first(state, refined_indices, MAX_DEPTH, 0, mesh_sizes); xyz_traverse(state, mesh_sizes, 0)){
    test[state.current_indice] = state.current_indice-state.block_beginning_indice;
  }

  pretty_print_named_array(test, 0, 2*(test_mesh_size*test_mesh_size*test_mesh_size));

  for(int i = 0; i < pow(test_mesh_size,3); i++){ CHECK_EQUAL(i,test[i]); }; //root region
  for(int i = pow(test_mesh_size,3); i < pow(test_mesh_size,3)*2; i++){ CHECK_EQUAL(i-pow(test_mesh_size,3),test[i]); }; //level 1

  std::fill(test, test + MESH_BUFFER_SIZE, 100);
  std::fill(order, order + MESH_BUFFER_SIZE, 100);

  //again, but ignoring ghosts
  for(init_state(state, mesh_sizes); breadth_first(state, refined_indices, MAX_DEPTH, 1, mesh_sizes); xyz_traverse(state, mesh_sizes, 1)){
    test[state.current_indice] = state.current_indice-state.block_beginning_indice;
  }

  pretty_print_named_array(test, 0, 2*(test_mesh_size*test_mesh_size*test_mesh_size));


  delete [] test;
  delete [] order;
  delete [] refined_indices;
}


TEST_GROUP(world_space_conversions){};

TEST(world_space_conversions, cell_world_lookup_test){

  float x,y,z;
  traverse_state state;
  int mesh_sizes[MAX_DEPTH];
  std::fill(mesh_sizes, mesh_sizes + MAX_DEPTH, 2);
  init_state(state, mesh_sizes);
  // refined_indices[1] = 30;

  cell_world_lookup(state, x, y, z);

  DOUBLES_EQUAL(-ROOT_WORLD_SCALE/2, x, 1e-5);
  DOUBLES_EQUAL(-ROOT_WORLD_SCALE/2, y, 1e-5);
  DOUBLES_EQUAL(-ROOT_WORLD_SCALE/2, z, 1e-5);


  xyz_traverse(state, mesh_sizes, 0);

  cell_world_lookup(state, x, y, z);
  pretty_print_named_value(state.x);

  DOUBLES_EQUAL(0, x, 1e-5);
  DOUBLES_EQUAL(-ROOT_WORLD_SCALE/2, y, 1e-5);
  DOUBLES_EQUAL(-ROOT_WORLD_SCALE/2, z, 1e-5);

}


TEST_GROUP(data_structure_benchmark){
};


TEST(data_structure_mesh, traverse_benchmark){
  int test_mesh_size = 10;

  int mesh_sizes[MAX_DEPTH];
  std::fill(mesh_sizes, mesh_sizes + MAX_DEPTH, test_mesh_size);


  int * test = new int[MESH_BUFFER_SIZE];
  int * refined_indices = new int[MESH_BUFFER_SIZE];
  std::fill(test, test + MESH_BUFFER_SIZE, 0);
  std::fill(refined_indices, refined_indices + MESH_BUFFER_SIZE, 0);


  int heap_end = mesh_sizes[0]*mesh_sizes[0]*mesh_sizes[0];
  for(int i = 0; i < pow(test_mesh_size,3); i++){
    refined_indices[i] = heap_end;//center-middle
    heap_end += pow(test_mesh_size,3);
  }

  traverse_state state;

  auto start = std::chrono::high_resolution_clock::now();

  for(init_state(state, mesh_sizes); breadth_first(state, refined_indices, MAX_DEPTH, 0, mesh_sizes); xyz_traverse(state, mesh_sizes, 0)){
    test[state.current_indice] = state.current_indice-state.block_beginning_indice;
  }

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << "Time taken by function: " << duration.count() << " microseconds\n";


  delete [] test;
  delete [] refined_indices;
}


#endif
