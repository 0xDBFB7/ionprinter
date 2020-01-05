#include "nyion.hpp"
#include "visualize.hpp"
#include "data_structure.hpp"


#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"

TEST_GROUP(MathGL)
{
};


TEST(MathGL, MathGL_Basic)
{
  mglGraph gr;
  gr.FPlot("sin(pi*x)");
  // gr.WriteFrame("test.png");
}

TEST_GROUP(OpenGL)
{
};

IGNORE_TEST(OpenGL, OpenGL_Basic)
{
  initialize_opengl();
  opengl_3d_mode();
  while(true){
    opengl_clear_screen();
    opengl_draw_axis_cross();
    opengl_test_prism();
    update_screen();
  }
}

TEST_GROUP(data_structure_mesh){
};
//

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


IGNORE_TEST(data_structure_mesh, traverse_tests){
  int mesh_sizes[MAX_DEPTH];
  std::fill(mesh_sizes, mesh_sizes + MAX_DEPTH, 3);

  int * potentials = new int[MESH_BUFFER_SIZE];
  int * refined_indices = new int[MESH_BUFFER_SIZE];
  std::fill(potentials, potentials + MESH_BUFFER_SIZE, 0);
  std::fill(refined_indices, refined_indices + MESH_BUFFER_SIZE, 0);

  traverse_state state;
  //first we encounter a ghost.
  breadth_first(state, refined_indices, 1, 0, mesh_sizes);



  delete [] potentials;
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
  pretty_print_named_array(potentials, 0, MESH_BUFFER_SIZE);

  CHECK_EQUAL(14,potentials[14]);
  CHECK_EQUAL(17,potentials[15]);

  CHECK_EQUAL(14,potentials[16]);
  CHECK_EQUAL(17,potentials[17]);


  delete [] potentials;
  delete [] refined_indices;
}



int main(int ac, char** av)
{
   return CommandLineTestRunner::RunAllTests(ac, av);
}
