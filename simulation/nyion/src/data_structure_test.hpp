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


TEST(data_structure_mesh, traverse_tests){
  int test_mesh_size = 2;
  int mesh_sizes[MAX_DEPTH];
  std::fill(mesh_sizes, mesh_sizes + MAX_DEPTH, test_mesh_size);

  int * test = new int[MESH_BUFFER_SIZE];
  int * refined_indices = new int[MESH_BUFFER_SIZE];
  std::fill(test, test + MESH_BUFFER_SIZE, 0);
  std::fill(refined_indices, refined_indices + MESH_BUFFER_SIZE, 0);

  int heap_end = mesh_sizes[0]*mesh_sizes[0]*mesh_sizes[0]+1;
  refined_indices[14] = heap_end;//center-middle

  traverse_state state;

  for(init_state(state, mesh_sizes); breadth_first(state, refined_indices, MAX_DEPTH, 0, mesh_sizes); xyz_traverse(state, mesh_sizes, 0)){
    //test[state.current_indice] = state.current_indice-state.block_beginning_indice;
    test[state.current_indice] = state.current_depth+1;
  }

  pretty_print_named_array(test, 0, 2*(test_mesh_size*test_mesh_size*test_mesh_size));

  delete [] test;
  delete [] refined_indices;
}


IGNORE_TEST(data_structure_mesh, data_structure_mesh_1d){
  int mesh_sizes[MAX_DEPTH];
  std::fill(mesh_sizes, mesh_sizes + MAX_DEPTH, 8);

  int * potentials = new int[MESH_BUFFER_SIZE];
  int * refined_indices = new int[MESH_BUFFER_SIZE];
  std::fill(potentials, potentials + MESH_BUFFER_SIZE, 0);
  std::fill(refined_indices, refined_indices + MESH_BUFFER_SIZE, 0);

  potentials[14] = 14;
  potentials[17] = 17;

  refined_indices[1] = 8;
  refined_indices[2] = 16;
  refined_indices[5] = 24;
  refined_indices[21] = 32;

  sync_ghosts(potentials,refined_indices,1,mesh_sizes);
  pretty_print_named_array(potentials, 0, 40);

  CHECK_EQUAL(14,potentials[14]);
  CHECK_EQUAL(17,potentials[15]);

  CHECK_EQUAL(14,potentials[16]);
  CHECK_EQUAL(17,potentials[17]);


  delete [] potentials;
  delete [] refined_indices;
}




TEST_GROUP(data_structure_benchmark){
};


TEST(data_structure_mesh, traverse_benchmark){
  int test_mesh_size = 100;

  int mesh_sizes[MAX_DEPTH];
  std::fill(mesh_sizes, mesh_sizes + MAX_DEPTH, test_mesh_size);


  int * test = new int[MESH_BUFFER_SIZE];
  int * refined_indices = new int[MESH_BUFFER_SIZE];
  std::fill(test, test + MESH_BUFFER_SIZE, 0);
  std::fill(refined_indices, refined_indices + MESH_BUFFER_SIZE, 0);

  int heap_end = mesh_sizes[0]*mesh_sizes[0]*mesh_sizes[0]+1;
  for(int i = 0; i < MAX_DEPTH; i++){
    refined_indices[14] = heap_end;//center-middle
  }

  traverse_state state;

  for(init_state(state, mesh_sizes); breadth_first(state, refined_indices, MAX_DEPTH, 0, mesh_sizes); xyz_traverse(state, mesh_sizes, 0)){
    test[state.current_indice] = state.current_indice-state.block_beginning_indice;
  }

  delete [] test;
  delete [] refined_indices;
}


#endif
