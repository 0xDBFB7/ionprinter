#include "nyion.hpp"
// #include "visualize.hpp"
//

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"

TEST_GROUP(FirstTestGroup)
{
};

int sample(HMGL gr, void *)
{
  mgl_rotate(gr,60,40,0);
  mgl_box(gr);
  return 0;

}

TEST(FirstTestGroup, FirstTest)
{
  mglGraph gr;
  gr.FPlot("sin(pi*x)");
  gr.WriteFrame("test.png");
  FAIL("Fail me!");
}



int main(int ac, char** av)
{

   return CommandLineTestRunner::RunAllTests(ac, av);
}
