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

TEST(data_structure_mesh, data_structure_mesh_1d){
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


  sync_ghosts(potentials,refined_indices,1);
  pretty_print_named_array(potentials, 0, MESH_BUFFER_SIZE);
  // dbg(potential);

  delete [] potentials;
  delete [] refined_indices;
}



int main(int ac, char** av)
{
   return CommandLineTestRunner::RunAllTests(ac, av);
}
