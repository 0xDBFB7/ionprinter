#include "nyion.hpp"
#include "visualize.hpp"


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

TEST(OpenGL, OpenGL_Basic)
{
  initialize_opengl();
  opengl_3d_mode();
  while(true){
    opengl_clear_screen();
    opengl_draw_axis_cross();
    opengl_test_prism();
    update_screen();
    int test[10];
    test[15] = 5;
    std::cout << test[15];
  }
}



int main(int ac, char** av)
{
   return CommandLineTestRunner::RunAllTests(ac, av);
}
