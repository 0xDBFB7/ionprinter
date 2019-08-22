#include "nyion.hpp"
// #include "visualize.hpp"
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"

TEST_GROUP(MG_GPU_OPERATORS)
{
};

TEST(MG_GPU_OPERATORS, multires_gauss_seidel_test)
{
  //test on non-square array
  cl::Buffer buffer_potentials(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ));
  cl::Buffer buffer_boundaries(context,CL_MEM_READ_WRITE,sizeof(int)*(SIZE_XYZ));

  float * potentials = new float[(SIZE_XYZ)];
  int * boundaries = new int[(SIZE_XYZ)];
  memset (potentials, 0, sizeof (float) * SIZE_XYZ); //zero out arrays
  memset (boundaries, 0, sizeof (int) * SIZE_XYZ);

  int res = 4;

  potentials[(SIZE_XY)*res+SIZE_X*res*2+res*2] = 200;
  boundaries[(SIZE_XY)*res+SIZE_X*res*2+res*2] = 1;

  queue.enqueueWriteBuffer(buffer_potentials,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),potentials);
  queue.enqueueWriteBuffer(buffer_boundaries,CL_TRUE,0,sizeof(int)*(SIZE_XYZ),boundaries);


  cl::Kernel gauss_seidel(program, "multires_gauss_seidel");
  gauss_seidel.setArg(0, buffer_potentials);
  gauss_seidel.setArg(1, buffer_boundaries);
  gauss_seidel.setArg(2, res);
  gauss_seidel.setArg(3, SIZE_X);
  gauss_seidel.setArg(4, SIZE_Y);
  auto t1 = std::chrono::high_resolution_clock::now();

  for(int i = 0; i < 10; i++){
    queue.enqueueNDRangeKernel(gauss_seidel,cl::NullRange,cl::NDRange((SIZE_X-(2*res))/res,(SIZE_Y-(2*res))/res,(SIZE_Z-(2*res))/res),cl::NullRange);
    queue.finish();
  }

  auto t2 = std::chrono::high_resolution_clock::now();

  cl::Kernel linterp(program, "multires_interpolate");
  linterp.setArg(0, buffer_potentials);
  linterp.setArg(1, res);
  linterp.setArg(2, SIZE_X);
  linterp.setArg(3, SIZE_Y);

  queue.enqueueNDRangeKernel(linterp,cl::NullRange,cl::NDRange((SIZE_X-(2*res))/res,(SIZE_Y-(2*res))/res,(SIZE_Z-(2*res))/res),cl::NullRange);
  queue.finish();

  res = 2;
  linterp.setArg(1, res);

  queue.enqueueNDRangeKernel(linterp,cl::NullRange,cl::NDRange((SIZE_X-(2*res))/res,(SIZE_Y-(2*res))/res,(SIZE_Z-(2*res))/res),cl::NullRange);
  queue.finish();

  res = 4;

  //read result C from the device to array C
  queue.enqueueReadBuffer(buffer_potentials,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),potentials);

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()/10.0;
  std::cout << "Gauss-Seidel on " << SIZE_X <<  " took " << duration << " us\n";

  display_array(potentials,SIZE_X,SIZE_Y,SIZE_Z,res);

  delete[] potentials;
  delete[] boundaries;
}


TEST(MG_GPU_OPERATORS, multires_linterp_test)
{
  //test on non-square array
  cl::Buffer buffer_potentials(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ));

  float * potentials = new float[(SIZE_XYZ)];
  memset (potentials, 0, sizeof (float) * SIZE_XYZ); //zero out arrays

  int res = 8;

  potentials[(SIZE_XY)*res+SIZE_X*res+res] = 200;
  // potentials[(SIZE_XY)+SIZE_X+2] = 1000;
  // boundaries[(SIZE_XY)+SIZE_X+2] = 1;

  queue.enqueueWriteBuffer(buffer_potentials,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),potentials);


  cl::Kernel linterp(program, "multires_interpolate");
  linterp.setArg(0, buffer_potentials);
  linterp.setArg(1, res);
  linterp.setArg(2, SIZE_X);
  linterp.setArg(3, SIZE_Y);
  auto t1 = std::chrono::high_resolution_clock::now();

  for(int i = 0; i < 10; i++){
    queue.enqueueNDRangeKernel(linterp,cl::NullRange,cl::NDRange((SIZE_X-(2*res))/res,(SIZE_Y-(2*res))/res,(SIZE_Z-(2*res))/res),cl::NullRange);
    queue.finish();
  }

  auto t2 = std::chrono::high_resolution_clock::now();

  //read result C from the device to array C
  queue.enqueueReadBuffer(buffer_potentials,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),potentials);

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()/10.0;
  std::cout << "Linterp on " << SIZE_X <<  " took " << duration << " us\n";

  // display_array(potentials,SIZE_X,SIZE_Y,SIZE_Z,res);

  delete[] potentials;
}


TEST(MG_GPU_OPERATORS, transfer_timing_test)
{

  cl::Buffer buffer_output(context,CL_MEM_READ_WRITE,sizeof(float)*SIZE_XYZ);
  cl::Buffer buffer_copy(context,CL_MEM_READ_WRITE,sizeof(float)*SIZE_XYZ);

  float * output = new float[(SIZE_XYZ)];


  auto t1 = std::chrono::high_resolution_clock::now();
  queue.enqueueWriteBuffer(buffer_output,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),output);
  queue.finish();
  auto t2 = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
  std::cout << "PCIe write took " << duration << " us\n";


  t1 = std::chrono::high_resolution_clock::now();
  queue.enqueueReadBuffer(buffer_output,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),output);
  queue.finish();
  t2 = std::chrono::high_resolution_clock::now();

  duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
  std::cout << "PCIe read took " << duration << " us\n";


  t1 = std::chrono::high_resolution_clock::now();
  queue.enqueueCopyBuffer(buffer_output,buffer_copy,0,0,sizeof(float)*(SIZE_XYZ));
  queue.finish();
  t2 = std::chrono::high_resolution_clock::now();

  duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
  std::cout << "Buffer copy took " << duration << " us\n";


  delete[] output;

}
//
// TEST(MG_GPU_OPERATORS, multigrid_test)
// {
//   const int MG_LEVELS = 4;//level 0 is the finest.
//   const int GS_CYCLES_PER_LEVEL = 3;//level 0 is the finest.
//   const int MG_CYCLES = 50;
//
//   //test on non-square array
//   cl::Buffer buffer_potentials(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ));
//   cl::Buffer buffer_residuals(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ));
//   cl::Buffer buffer_boundaries(context,CL_MEM_READ_WRITE,sizeof(int)*(SIZE_XYZ));
//
//   float * potentials = new float[(SIZE_XYZ)];
//   float * residuals = new float[(SIZE_XYZ)];
//   int * boundaries = new int[(SIZE_XYZ)];
//   memset (potentials, 0, sizeof (float) * SIZE_XYZ); //zero out arrays
//   memset (residuals, 0, sizeof (float) * SIZE_XYZ); //zero out arrays
//   memset (boundaries, 0, sizeof (int) * SIZE_XYZ);
//
//   for(int i = 0; i < SIZE_X; i++){
//     potentials[(SIZE_XY)*2+SIZE_X*4+i] = 200;
//     boundaries[(SIZE_XY)*2+SIZE_X*4+i] = 1;
//   }
//
//   queue.enqueueWriteBuffer(buffer_potentials,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),potentials);
//   queue.enqueueWriteBuffer(buffer_boundaries,CL_TRUE,0,sizeof(int)*(SIZE_XYZ),boundaries);
//
//   int res = 1;
//
//   cl::Kernel gauss_seidel(program, "multires_gauss_seidel");
//   gauss_seidel.setArg(0, buffer_potentials);
//   gauss_seidel.setArg(1, buffer_boundaries);
//   gauss_seidel.setArg(2, res);
//   gauss_seidel.setArg(3, SIZE_X);
//   gauss_seidel.setArg(4, SIZE_Y);
//
//   cl::Kernel linterp(program, "multires_interpolate");
//   linterp.setArg(0, buffer_potentials);
//   linterp.setArg(1, res);
//   linterp.setArg(2, SIZE_X);
//   linterp.setArg(3, SIZE_Y);
//
//   cl::Kernel residual_dist(program, "dist");
//   residual_dist.setArg(0, buffer_potentials);
//   residual_dist.setArg(1, buffer_residuals);
//   residual_dist.setArg(2, buffer_residuals);
//
//   auto t1 = std::chrono::high_resolution_clock::now();
//
//   for(int cycles = 0; cycles < MG_CYCLES; cycles++){
//     for(int level = MG_LEVELS; level > -1; level--){
//       res = 4;
//       for(int i = 0; i < GS_CYCLES_PER_LEVEL; i++){
//         printf("Relaxing on %i\n",res);
//         queue.enqueueNDRangeKernel(gauss_seidel,cl::NullRange,cl::NDRange((SIZE_X-(2*res))/res,(SIZE_Y-(2*res))/res,(SIZE_Z-(2*res))/res),cl::NullRange);
//       }
//       queue.enqueueNDRangeKernel(linterp,cl::NullRange,cl::NDRange((SIZE_X-(2*res))/res,(SIZE_Y-(2*res))/res,(SIZE_Z-(2*res))/res),cl::NullRange);
//       queue.finish();
//     }
//
//     queue.enqueueCopyBuffer(buffer_potentials,buffer_residuals,0,0,sizeof(float)*(SIZE_XYZ));
//     queue.finish();
//     queue.enqueueNDRangeKernel(gauss_seidel,cl::NullRange,cl::NDRange((SIZE_X-(2*res))/res,(SIZE_Y-(2*res))/res,(SIZE_Z-(2*res))/res),cl::NullRange);
//
//     queue.enqueueNDRangeKernel(residual_dist,cl::NullRange,cl::NDRange(SIZE_XYZ),cl::NullRange);
//
//     queue.enqueueReadBuffer(buffer_residuals,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),residuals);
//     double max = *std::max_element(residuals, residuals+SIZE_XYZ);
//     printf("Residual: %f\n",max);
//
//   }
//
//   auto t2 = std::chrono::high_resolution_clock::now();
//
//
//
//
//   queue.enqueueReadBuffer(buffer_potentials,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),potentials);
//
//   display_array(potentials,SIZE_X,SIZE_Y,SIZE_Z);
//
//   //read result C from the device to array C
//
//   auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()/10.0;
//   std::cout << "Multigrid on " << SIZE_X <<  " took " << duration << " us\n";
//
//
//   delete[] potentials;
//   delete[] residuals;
//   delete[] boundaries;
// }

int main(int ac, char** av){
  init_OpenCL();
  return RUN_ALL_TESTS(ac, av);

}
