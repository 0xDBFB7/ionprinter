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


  //read result C from the device to array C
  queue.enqueueReadBuffer(buffer_potentials,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),potentials);

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()/10.0;
  std::cout << "Gauss-Seidel on " << SIZE_X <<  " took " << duration << " us\n";

  // display_array(potentials,SIZE_X,SIZE_Y,SIZE_Z,res);

  delete[] potentials;
  delete[] boundaries;
}


TEST(MG_GPU_OPERATORS, multires_linterp_test)
{
  //test on non-square array
  cl::Buffer buffer_potentials(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ));
  cl::Buffer buffer_boundaries(context,CL_MEM_READ_WRITE,sizeof(int)*(SIZE_XYZ));

  float * potentials = new float[(SIZE_XYZ)];
  float * boundaries = new float[(SIZE_XYZ)];

  memset (potentials, 0, sizeof (float) * SIZE_XYZ); //zero out arrays

  int res = 4;

  // potentials[(SIZE_XY)*res+SIZE_X*res+res] = 200;
  // potentials[(SIZE_XY)+SIZE_X+2] = 1000;
  // boundaries[(SIZE_XY)+SIZE_X+2] = 1;

  queue.enqueueWriteBuffer(buffer_potentials,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),potentials);
  queue.enqueueWriteBuffer(buffer_boundaries,CL_TRUE,0,sizeof(int)*(SIZE_XYZ),boundaries);


  cl::Kernel linterp(program, "multires_interpolate");
  linterp.setArg(0, buffer_potentials);
  linterp.setArg(1, buffer_boundaries);
  linterp.setArg(2, res);
  linterp.setArg(3, SIZE_X);
  linterp.setArg(4, SIZE_Y);
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
  delete[] boundaries;

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

TEST(MG_GPU_OPERATORS, multigrid_test)
{
  // int MG_LEVELS = 4;//level 0 is the finest.
  // const int GS_CYCLES_PER_LEVEL = 2;//level 0 is the finest.
  const int MG_CYCLES = 30;

  /* -----------------------------------------------------------------------------
  GPU kernel initialization
  ----------------------------------------------------------------------------- */
  cl::Kernel gauss_seidel(program, "multires_gauss_seidel");
  cl::Kernel linterp(program, "multires_interpolate");
  cl::Kernel multires_restrict(program, "multires_restrict");
  cl::Kernel residual_subtract(program, "subtract");
  cl::Kernel residual_add(program, "add");
  gauss_seidel.setArg(3, SIZE_X);
  gauss_seidel.setArg(4, SIZE_Y);
  linterp.setArg(2, SIZE_X);
  linterp.setArg(3, SIZE_Y);
  multires_restrict.setArg(2, SIZE_X);
  multires_restrict.setArg(3, SIZE_Y);
  /* -----------------------------------------------------------------------------
  GPU buffer allocation
  ----------------------------------------------------------------------------- */
  cl::Buffer buffer_U(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ)); //potential approximation
  cl::Buffer buffer_b(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ)); //boundary condition values
  cl::Buffer buffer_r(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ)); //residuals
  cl::Buffer buffer_v(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ)); //correction
  /* -----------------------------------------------------------------------------
  Local buffer allocation
  ----------------------------------------------------------------------------- */
  float * U = new float[(SIZE_XYZ)];
  float * r = new float[(SIZE_XYZ)];
  float * v = new float[(SIZE_XYZ)];
  float * b = new float[(SIZE_XYZ)];
  memset (U, 0, sizeof (float) * SIZE_XYZ); //zero out arrays
  memset (r, 0, sizeof (float) * SIZE_XYZ); //zero out arrays
  memset (b, 0, sizeof (float) * SIZE_XYZ);
  memset (v, 0, sizeof (float) * SIZE_XYZ);
  /* -----------------------------------------------------------------------------
  Demo problem setup
  ----------------------------------------------------------------------------- */
  for(int i = 0; i < SIZE_X/2; i++){
    b[(SIZE_XY*8)+SIZE_X*8+i+8] = 200;
  }
  /* -----------------------------------------------------------------------------
  Copy problem to GPU memory
  ----------------------------------------------------------------------------- */
  queue.enqueueWriteBuffer(buffer_U,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),U);
  queue.enqueueWriteBuffer(buffer_b,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),b);
  /* -----------------------------------------------------------------------------
  Max stores the greatest value in the residual; this is cheaper than finding the norm
  ----------------------------------------------------------------------------- */
  double max = 0;
  auto t1 = std::chrono::high_resolution_clock::now();

  for(int cycles = 0; cycles < MG_CYCLES; cycles++){
    queue.enqueueFillBuffer(buffer_v,0,0,sizeof(float)*(SIZE_XYZ)); //wipe correction buffer
    /* -----------------------------------------------------------------------------
    Store a copy of the previous iteration
    ----------------------------------------------------------------------------- */
    queue.enqueueCopyBuffer(buffer_U,buffer_r,0,0,sizeof(float)*(SIZE_XYZ));//copy residuals
    /* -----------------------------------------------------------------------------
    Compute the finest gauss-seidel iteration
    ----------------------------------------------------------------------------- */
    int res = 1; //a full resolution fine grid cycle
    gauss_seidel.setArg(0, buffer_U);
    gauss_seidel.setArg(1, buffer_b);
    gauss_seidel.setArg(2, res);
    queue.enqueueNDRangeKernel(gauss_seidel,cl::NullRange,cl::NDRange((SIZE_X-(2*res))/res,(SIZE_Y-(2*res))/res,(SIZE_Z-(2*res))/res),cl::NullRange);
    /* -----------------------------------------------------------------------------
    Subtract the first iteration from the new potentials to obtain the residuals
    ----------------------------------------------------------------------------- */
    residual_subtract.setArg(0, buffer_U);
    residual_subtract.setArg(1, buffer_r);
    residual_subtract.setArg(2, buffer_r);
    queue.enqueueNDRangeKernel(residual_subtract,cl::NullRange,cl::NDRange(SIZE_XYZ),cl::NullRange);
    /* -----------------------------------------------------------------------------
    Read residuals from GPU for diagnostics (optional)
    ----------------------------------------------------------------------------- */
    queue.enqueueReadBuffer(buffer_r,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),r);
    queue.finish();
    max = *std::max_element(r, r+SIZE_XYZ);
    printf("Residual: %f\n",max);

    /* -----------------------------------------------------------------------------
    Cycle down from coarse level
    ----------------------------------------------------------------------------- */
    linterp.setArg(0, buffer_v);
    linterp.setArg(1, res);
    gauss_seidel.setArg(0, buffer_v);
    gauss_seidel.setArg(1, buffer_r);
    gauss_seidel.setArg(2, res);
    for(int level = 4; level > -1; level--){
      res = pow(2,level);
      for(int i = 0; i < res; i++){ //as many cycles as the resolution is a good approximation
        queue.enqueueNDRangeKernel(gauss_seidel,cl::NullRange,cl::NDRange((SIZE_X-(2*res))/res,(SIZE_Y-(2*res))/res,(SIZE_Z-(2*res))/res),cl::NullRange);
      }
      if(res > 1){
        queue.enqueueNDRangeKernel(linterp,cl::NullRange,cl::NDRange((SIZE_X-(2*res))/res,(SIZE_Y-(2*res))/res,(SIZE_Z-(2*res))/res),cl::NullRange);
      }
      queue.finish();
    }

    /* -----------------------------------------------------------------------------
    Apply correction to fine grid.
    ----------------------------------------------------------------------------- */
    residual_add.setArg(0, buffer_U);
    residual_add.setArg(1, buffer_v);
    residual_add.setArg(2, buffer_U);
    queue.enqueueNDRangeKernel(residual_add,cl::NullRange,cl::NDRange(SIZE_XYZ),cl::NullRange);
  }


  display_array(v,SIZE_X,SIZE_Y,SIZE_Z,8);


  auto t2 = std::chrono::high_resolution_clock::now();


  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()/10.0;
  std::cout << "Multigrid on " << SIZE_X <<  "^3 took " << duration << " us\n";


  delete[] U;
  delete[] r;
  delete[] v;
  delete[] b;
}

int main(int ac, char** av){
  init_OpenCL();
  return RUN_ALL_TESTS(ac, av);

}
