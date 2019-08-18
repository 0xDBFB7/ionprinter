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

  // display_array(potentials,SIZE_X,SIZE_Y,SIZE_Z);

  delete[] potentials;
  delete[] boundaries;
}

TEST(MG_GPU_OPERATORS, weighted_restrict_test)
{
  int output_size = ((SIZE_X/2)*(SIZE_Y/2)*(SIZE_Z/2));
  //test on non-square array
  cl::Buffer buffer_input(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ));
  cl::Buffer buffer_output(context,CL_MEM_READ_WRITE,sizeof(float)*output_size);

  float * input = new float[(SIZE_XYZ)];
  float * output = new float[(output_size)];
  memset (input, 0, sizeof (float) * SIZE_XYZ); //zero out arrays
  memset (output, 0, sizeof (float) * output_size);

  for(int x = (SIZE_X*SIZE_Y); x < (SIZE_X*2 + (SIZE_X*SIZE_Y)); x++){
    input[x] = 100;
  }

  queue.enqueueWriteBuffer(buffer_input,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),input);
  queue.enqueueWriteBuffer(buffer_output,CL_TRUE,0,sizeof(float)*(output_size),output);

  cl::Kernel weighted_restrict(program, "weighted_restrict");
  weighted_restrict.setArg(0, buffer_input);
  weighted_restrict.setArg(1, buffer_output);

  auto t1 = std::chrono::high_resolution_clock::now();

  for(int i = 0; i < 10; i++){
    queue.enqueueNDRangeKernel(weighted_restrict,cl::NullRange,cl::NDRange((SIZE_X/2)-2,(SIZE_Y/2)-2,(SIZE_Z/2)-2),cl::NullRange);
    queue.finish();
  }

  auto t2 = std::chrono::high_resolution_clock::now();

  //read result C from the device to array C
  queue.enqueueReadBuffer(buffer_output,CL_TRUE,0,sizeof(float)*(output_size),output);

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()/10.0;
  std::cout << "Weighted restrict on " << SIZE_X <<  " took " << duration << " us\n";


  // display_array(output,(SIZE_X/2),(SIZE_Y/2),(SIZE_Z/2));


  // for(int x = ((SIZE_X/2)*(SIZE_Y/2)); x < (SIZE_X + ((SIZE_X/2)*(SIZE_Y/2))); x++){
  //   std::cout << output[x+(SIZE_X/2)] << "\n";
  // }

  delete[] input;
  delete[] output;
}



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
  //
  // for(int x = 0; x < (SIZE_X); x++){
  //   std::cout << output[x] << "\n";
  // }

  delete[] input;
  delete[] output;
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
// TEST(MG_GPU_OPERATORS, residual_test)
// {
//   float * test = new float[SIZE_XYZ];
//   float * out = new float[1];
//   memset (test, 0, sizeof (float) * SIZE_XYZ); //zero out arrays
//   memset (out, 0, sizeof (float)); //zero out arrays
//
//   test[0] = 1;
//   test[1] = 1;
//
//   cl::Buffer function_outputs(context,CL_MEM_READ_WRITE,sizeof(float));
//   cl::Buffer test_buf(context,CL_MEM_READ_WRITE,sizeof(float)*SIZE_XYZ);
//   cl::Buffer test_buf2(context,CL_MEM_READ_WRITE,sizeof(float)*SIZE_XYZ);
//
//
//
//   queue.enqueueWriteBuffer(test_buf,CL_TRUE,0,sizeof(float)*SIZE_XYZ,test); //write the problem to the finest grid
//   queue.finish();
//
//   test[0] = 0;
//   test[1] = 0;
//
//   queue.enqueueWriteBuffer(test_buf2,CL_TRUE,0,sizeof(float)*SIZE_XYZ,test); //write the problem to the finest grid
//   queue.finish();
//
//   cl::Kernel dist(program, "dist");
//   dist.setArg(0, test_buf);
//   dist.setArg(1, test_buf2);
//   dist.setArg(2, function_outputs);
//
//   auto t1 = std::chrono::high_resolution_clock::now();
//
//   queue.enqueueNDRangeKernel(dist,cl::NullRange,cl::NDRange(sizeof(float)*SIZE_XYZ),cl::NullRange);
//   queue.finish();
//
//   auto t2 = std::chrono::high_resolution_clock::now();
//
//   auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()/10.0;
//   std::cout << "Residual on " << SIZE_X <<  " took " << duration << " us\n";
//
//
//   queue.enqueueReadBuffer(function_outputs,CL_TRUE,0,sizeof(float),out);
//   queue.finish();
//
//   std::cout << out[0];
//
//   delete[] test;
//   delete[] out;
//
// }

TEST(MG_GPU_OPERATORS, multigrid_test)
{

  const int MG_LEVELS = 5;//level 0 is the finest.

  float * potentials = new float[(SIZE_XYZ)];
  int * boundaries = new int[(SIZE_XYZ)];
  memset (potentials, 0, sizeof (float) * SIZE_XYZ); //zero out arrays
  memset (boundaries, 0, sizeof (int) * SIZE_XYZ);

  cl::Buffer function_outputs(context,CL_MEM_READ_WRITE,sizeof(float));

  cl::Buffer buffer_boundaries(context,CL_MEM_READ_WRITE,sizeof(int)*(SIZE_XYZ));
  queue.enqueueWriteBuffer(buffer_boundaries,CL_TRUE,0,sizeof(int)*(SIZE_XYZ),boundaries); //write the problem to the finest grid

  std::vector<cl::Buffer> buffer_potentials(MG_LEVELS);
  std::vector<cl::Buffer> buffer_residuals(MG_LEVELS);

  for(int level = 0; level < MG_LEVELS; level++){
    buffer_potentials[level] = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ));
    buffer_residuals[level] = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ));
    queue.enqueueFillBuffer(buffer_potentials[level],0.0,0,sizeof(float)*(SIZE_XYZ));
    queue.enqueueFillBuffer(buffer_residuals[level],0.0,0,sizeof(float)*(SIZE_XYZ)); //just establish a defined state
  }

  // potentials[(SIZE_XY)+SIZE_X+1] = 10;
  // boundaries[(SIZE_XY)+SIZE_X+1] = 1;
  //
  queue.enqueueWriteBuffer(buffer_potentials[0],CL_TRUE,0,sizeof(float)*(SIZE_XYZ),potentials); //write the problem to the finest grid
  queue.finish();

  // const int GS_CYCLES = 5; //level 0 is the finest.


  // for(int level = 0; level < MG_LEVELS; level++){

  cl::Kernel gauss_seidel(program, "gauss_seidel");
  gauss_seidel.setArg(0, buffer_potentials);
  gauss_seidel.setArg(1, buffer_boundaries);

  // auto t1 = std::chrono::high_resolution_clock::now();
  //
  // for(int i = 0; i < 10; i++){
  //   queue.enqueueNDRangeKernel(gauss_seidel,cl::NullRange,cl::NDRange(SIZE_X-2,SIZE_Y-2,SIZE_Z-2),cl::NullRange);
  //   queue.finish();
  // }
  //
  // auto t2 = std::chrono::high_resolution_clock::now();
  //
  // //read result C from the device to array C
  // queue.enqueueReadBuffer(buffer_potentials,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),potentials);
  //
  // auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()/10.0;
  // std::cout << "Gauss-Seidel on " << SIZE_X <<  " took " << duration << " us\n";
  //
  // // display_array(potentials,SIZE_X,SIZE_Y,SIZE_Z);
  //
  delete[] potentials;
  delete[] boundaries;
}

int main(int ac, char** av){
  init_OpenCL();
  return RUN_ALL_TESTS(ac, av);

}
