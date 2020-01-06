#include "nyion.hpp"
#include "visualize.hpp"
#include "data_structure.hpp"
#include "unrolled_operations.hpp"

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"

#include "data_structure_test.hpp"

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

TEST_GROUP(CUDA)
{
};

TEST(CUDA, CUDA_basic)
{

  for(int i = 0; i < 100; i++){ test_cuda_2();}

}



int main(int ac, char** av)
{
   return CommandLineTestRunner::RunAllTests(ac, av);
}
