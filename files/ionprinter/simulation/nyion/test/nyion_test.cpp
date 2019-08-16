#include "nyion.hpp"
// #include "visualize.hpp"
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"

TEST_GROUP(MG_GPU_OPERATORS)
{
};

TEST(MG_GPU_OPERATORS, gauss_seidel_test)
{
  //test on non-square array
  cl::Buffer buffer_potentials(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ));
  cl::Buffer buffer_boundaries(context,CL_MEM_READ_WRITE,sizeof(int)*(SIZE_XYZ));

  float * potentials = new float[(SIZE_XYZ)];
  int * boundaries = new int[(SIZE_XYZ)];
  memset (potentials, 0, sizeof (float) * SIZE_XYZ); //zero out arrays
  memset (boundaries, 0, sizeof (int) * SIZE_XYZ);

  potentials[(SIZE_XY)+SIZE_X+1] = 10;
  boundaries[(SIZE_XY)+SIZE_X+1] = 1;

  queue.enqueueWriteBuffer(buffer_potentials,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),potentials);
  queue.enqueueWriteBuffer(buffer_boundaries,CL_TRUE,0,sizeof(int)*(SIZE_XYZ),boundaries);

  cl::Kernel gauss_seidel(program, "gauss_seidel");
  gauss_seidel.setArg(0, buffer_potentials);
  gauss_seidel.setArg(1, buffer_boundaries);

  auto t1 = std::chrono::high_resolution_clock::now();

  for(int i = 0; i < 10; i++){
    queue.enqueueNDRangeKernel(gauss_seidel,cl::NullRange,cl::NDRange(SIZE_X-2,SIZE_Y-2,SIZE_Z-2),cl::NullRange);
    queue.finish();
  }

  auto t2 = std::chrono::high_resolution_clock::now();

  //read result C from the device to array C
  queue.enqueueReadBuffer(buffer_potentials,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),potentials);

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()/10.0;
  std::cout << "Gauss-Seidel on " << SIZE_X <<  " took " << duration << " us\n";

  display_array(potentials,SIZE_X,SIZE_Y,SIZE_Z);

  delete[] potentials;
  delete[] boundaries;
}
//
// TEST(MG_GPU_OPERATORS, weighted_restrict_test)
// {
//   int output_size = ((SIZE_X/2)*(SIZE_Y/2)*(SIZE_Z/2));
//   //test on non-square array
//   cl::Buffer buffer_input(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ));
//   cl::Buffer buffer_output(context,CL_MEM_READ_WRITE,sizeof(int)*output_size);
//
//   float * input = new float[(SIZE_XYZ)];
//   float * output = new float[(output_size)];
//   memset (input, 0, sizeof (float) * SIZE_XYZ); //zero out arrays
//   memset (output, 0, sizeof (float) * output_size);
//
//   queue.enqueueWriteBuffer(buffer_input,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),input);
//   queue.enqueueWriteBuffer(buffer_output,CL_TRUE,0,sizeof(float)*(output_size),output);
//
//   cl::Kernel weighted_restrict(program, "weighted_restrict");
//   weighted_restrict.setArg(0, buffer_input);
//   weighted_restrict.setArg(1, buffer_output);
//
//   auto t1 = std::chrono::high_resolution_clock::now();
//
//   for(int i = 0; i < 10; i++){
//     queue.enqueueNDRangeKernel(weighted_restrict,cl::NullRange,cl::NDRange((SIZE_X/2)-2,(SIZE_Y/2)-2,(SIZE_Z/2)-2),cl::NullRange);
//     queue.finish();
//   }
//
//   auto t2 = std::chrono::high_resolution_clock::now();
//
//   //read result C from the device to array C
//   queue.enqueueReadBuffer(buffer_output,CL_TRUE,0,sizeof(float)*(output_size),output);
//
//   auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()/10.0;
//   std::cout << "Weighted restrict on " << SIZE_X <<  " took " << duration << " us\n";
//
//   display_array(output,(SIZE_X/2),(SIZE_Y/2),(SIZE_Z/2));
//
//   delete[] input;
//   delete[] output;
// }



TEST(MG_GPU_OPERATORS, interpolate_test)
{
  int input_size = ((SIZE_X/2)*(SIZE_Y/2)*(SIZE_Z/2));
  //test on non-square array
  cl::Buffer buffer_input(context,CL_MEM_READ_WRITE,sizeof(float)*(input_size));
  cl::Buffer buffer_output(context,CL_MEM_READ_WRITE,sizeof(float)*SIZE_XYZ);

  float * input = new float[(input_size)];
  float * output = new float[(SIZE_XYZ)];
  memset (input, 0, sizeof (float) * input_size); //zero out arrays
  memset (output, 0, sizeof (float) * SIZE_XYZ);

  for(int x = 0; x < (SIZE_X); x++){
    input[x] = x;
  }

  queue.enqueueWriteBuffer(buffer_input,CL_TRUE,0,sizeof(float)*(input_size),input);
  queue.enqueueWriteBuffer(buffer_output,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),output);

  cl::Kernel interpolate(program, "interpolate");
  interpolate.setArg(0, buffer_input);
  interpolate.setArg(1, buffer_output);

  auto t1 = std::chrono::high_resolution_clock::now();

  for(int i = 0; i < 10; i++){
    queue.enqueueNDRangeKernel(interpolate,cl::NullRange,cl::NDRange((SIZE_X/2)-1,(SIZE_Y/2)-1,(SIZE_Z/2)-1),cl::NullRange);
    queue.finish();
  }

  auto t2 = std::chrono::high_resolution_clock::now();

  //read result C from the device to array C
  queue.enqueueReadBuffer(buffer_output,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),output);

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()/10.0;
  std::cout << "Interpolate on " << SIZE_X <<  " took " << duration << " us\n";

  // display_array(output,SIZE_X,SIZE_Y,SIZE_Z);

  for(int x = 0; x < (SIZE_X); x++){
    std::cout << output[x] << "\n";
  }

  delete[] input;
  delete[] output;
}


int main(int ac, char** av){
  init_OpenCL();
  return RUN_ALL_TESTS(ac, av);

}
