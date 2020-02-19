#include "nyion.hpp"
#include "visualize.hpp"

#include "host_data_structure.hpp"
#include "device_data_structure.hpp"

#include "host_data_structure_test.hpp"
#include "device_data_structure_test.hpp"

#include "gtest/gtest.h"

//rm -r * && cmake ../
//make -j16 && ./test/nyion_test

//this file calls the modules' respective test functions.


TEST(MathGL, MathGL_Basic)
{
    mglGraph gr;
    gr.FPlot("sin(pi*x)"); //recall: array linking:
    // double *a = new double[50];
    // for(int i=0;i<50;i++)   a[i] = sin(M_PI*i/49.);
    // mglData y;
    // y.Link(a,50);
  // gr.WriteFrame("test.png");

}
//
// TEST_GROUP(OpenGL)
// {
// };
//
// IGNORE_TEST(OpenGL, OpenGL_Basic)
// {
//   initialize_opengl();
//   opengl_3d_mode();
//   while(true){
//     opengl_clear_screen();
//     opengl_draw_axis_cross();
//     opengl_test_prism();
//     update_screen();
//   }
// }


//
// TEST(CUDA, CUDA_basic)
// {
//
//   float * x = new float[10];
//   float * y = new float[10];
//
//   // test_cuda(x);
//
//   float *d_x;
//
//   gpu_error_check( cudaMalloc(&d_x, 100*sizeof(float)));
//
//
//   pretty_print_named_array(x,0,10);
//
//   delete [] x;
//   delete [] y;
// }



int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    // return CommandLineTestRunner::RunAllTests(ac, av);
    return RUN_ALL_TESTS();
}
