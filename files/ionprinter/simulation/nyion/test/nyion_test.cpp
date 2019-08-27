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
    queue.enqueueNDRangeKernel(gauss_seidel,cl::NullRange,cl::NDRange(1),cl::NullRange);
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
  cl::Buffer buffer_r1(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ)); //residuals, copy - slmg doesn't require this array, but
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
  for(int x = 40; x < 50; x++){
    for(int y = 40; y < 50; y++){
      b[(SIZE_XY*8)+SIZE_X*y+x] = 1;
    }
  }
  /* -----------------------------------------------------------------------------
  Copy problem to GPU memory
  ----------------------------------------------------------------------------- */
  queue.enqueueWriteBuffer(buffer_U,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),U);
  queue.enqueueWriteBuffer(buffer_b,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),b);
  /* -----------------------------------------------------------------------------
  Max stores the greatest value in the residual; this is cheaper than finding the norm
  ----------------------------------------------------------------------------- */
  // double max = 0;

  const int MG_CYCLES = 30;
  int MG_LEVELS = 12;//level 0 is the finest.
  int resolutions[] = {32,16,8,16,8,4,8,4,2,4,2,1};

  residual_add.setArg(0, buffer_U);
  residual_add.setArg(1, buffer_v);
  residual_add.setArg(2, buffer_U);

  residual_subtract.setArg(0, buffer_U);
  residual_subtract.setArg(1, buffer_r);
  residual_subtract.setArg(2, buffer_r);

  float previous_norm = 1;

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
    queue.enqueueNDRangeKernel(gauss_seidel,cl::NullRange,cl::NDRange(1),cl::NullRange);
    /* -----------------------------------------------------------------------------
    Subtract the first iteration from the new potentials to obtain the residuals
    ----------------------------------------------------------------------------- */

    queue.enqueueNDRangeKernel(residual_subtract,cl::NullRange,cl::NDRange(SIZE_XYZ),cl::NullRange);
    /* -----------------------------------------------------------------------------
    Read residuals from GPU for diagnostics (optional)
    ----------------------------------------------------------------------------- */
    queue.enqueueReadBuffer(buffer_r,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),r);
    queue.finish();
    float norm = sqrt(std::inner_product(r, r+SIZE_XYZ, r, 0.0L));
    printf("Residual: %f\nConvergence Factor: %f\n",norm,norm/previous_norm);
    previous_norm = norm;
    /* -----------------------------------------------------------------------------
    Cycle down from coarse level
    ----------------------------------------------------------------------------- */
    linterp.setArg(0, buffer_v);
    multires_restrict.setArg(0, buffer_r1);
    gauss_seidel.setArg(0, buffer_v);
    gauss_seidel.setArg(1, buffer_r1);

    for(int level = 0; level < MG_LEVELS; level++){
      res = resolutions[level];
      printf("res: %i\n",res);
      gauss_seidel.setArg(2, res);
      linterp.setArg(1, res);
      multires_restrict.setArg(1, res);
      queue.enqueueCopyBuffer(buffer_r,buffer_r1,0,0,sizeof(float)*(SIZE_XYZ));//copy residuals
      //todo: faster copy could be implemented - theta subtract?
      if(res > 1){
        queue.enqueueNDRangeKernel(multires_restrict,cl::NullRange,cl::NDRange((SIZE_X-(2*res))/res,(SIZE_Y-(2*res))/res,(SIZE_Z-(2*res))/res),cl::NullRange);
      }
      for(int i = 0; i < resolutions[level]; i++){ //as many cycles as the resolution is a good approximation
        queue.enqueueNDRangeKernel(gauss_seidel,cl::NullRange,cl::NDRange(1),cl::NullRange);
      }
      if(res > 1){
        queue.enqueueNDRangeKernel(linterp,cl::NullRange,cl::NDRange((SIZE_X-(2*res))/res,(SIZE_Y-(2*res))/res,(SIZE_Z-(2*res))/res),cl::NullRange);
      }
      queue.finish();
    }

    queue.enqueueReadBuffer(buffer_v,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),v);
    queue.enqueueReadBuffer(buffer_U,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),U);
    queue.finish();
    display_array(v,SIZE_X,SIZE_Y,SIZE_Z,8);

    /* -----------------------------------------------------------------------------
    Apply correction to fine grid.
    ----------------------------------------------------------------------------- */
    queue.enqueueNDRangeKernel(residual_add,cl::NullRange,cl::NDRange(SIZE_XYZ),cl::NullRange);
  }


  auto t2 = std::chrono::high_resolution_clock::now();

  // display_array(v,SIZE_X,SIZE_Y,SIZE_Z,8);

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()/1.0;
  std::cout << "Multigrid on " << SIZE_X <<  "^3 took " << duration << " us\n";


  delete[] U;
  delete[] r;
  delete[] v;
  delete[] b;
}



// TEST(laplace_tests,opengl_simple_boundary){
//     double mesh_bounds[6] = {0,0.05,0,0.05,0,0.05};
//     root_mesh_geometry mesh_geometry(mesh_bounds, 0.003);
//
//     std::vector<std::vector<double>> potentials;
//     std::vector<std::vector<int>> boundaries;
//
//     double mesh_active_bounds[6] = {0,0.05,0,0.06,0,0.05};
//
//     enable_mesh_region(potentials,mesh_active_bounds,mesh_geometry,2);
//     enable_mesh_region(boundaries,mesh_active_bounds,mesh_geometry,2);
//
//     potentials[0][1] = -1000;
//     boundaries[0][1] = 1;
//     potentials[10][1] = 1000;
//     boundaries[10][1] = 1;
//     initialize_opengl(mesh_geometry);
//     std::vector<double> test_graph = {10,-10,0.3};
//
//     while(true){
//
//       opengl_switch_to_graph_window();
//       test_graph.push_back(10);
//       opengl_graph_1d_vector(test_graph,"Test",0);
//
//       opengl_2d_mode();
//
//       opengl_switch_to_mesh_window();
//
//       opengl_3d_mode();
//
//       opengl_apply_camera_rotation();
//
//       opengl_draw_axis_cross();
//
//       draw_geometry_outline(mesh_geometry);
//       gauss_seidel(potentials, boundaries, mesh_geometry, 5, 1, 0);
//       draw_mesh(potentials,mesh_geometry);
//
//       opengl_2d_mode();
//
//
//       update_screen();
//
//       opengl_switch_to_graph_window();
//       opengl_clear_screen();
//
//       opengl_switch_to_mesh_window();
//       opengl_clear_screen();
//       // update_screen();
//        bool valid = false;
//       printf("%f",relative_mesh_value(potentials,idx(10,0,16,17,17),16,16,16,0,0,-1,mesh_geometry,valid));
//
//     }
//
//
// }


int main(int ac, char** av){
  init_OpenCL();
  return RUN_ALL_TESTS(ac, av);

}
