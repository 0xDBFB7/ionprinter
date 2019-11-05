#include "nyion.hpp"
// #include "visualize.hpp"
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"

TEST_GROUP(MG_GPU_OPERATORS)
{
};


TEST(MG_GPU_OPERATORS, jacobi_test)
{
  //test on non-square array
  cl::Buffer buffer_potentials(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ));
  cl::Buffer buffer_boundaries(context,CL_MEM_READ_WRITE,sizeof(int)*(SIZE_XYZ));
  cl::Buffer buffer_RHS(context,CL_MEM_READ_WRITE,sizeof(int)*(SIZE_XYZ));
  cl::Buffer buffer_output(context,CL_MEM_READ_WRITE,sizeof(int)*(SIZE_XYZ));

  float * potentials = new float[(SIZE_XYZ)];
  int * boundaries = new int[(SIZE_XYZ)];
  memset (potentials, 0, sizeof (float) * SIZE_XYZ); //zero out arrays
  memset (boundaries, 0, sizeof (int) * SIZE_XYZ);

  int res = 1;

  potentials[(SIZE_XY)*res+SIZE_X*res*2+res*2] = 200;
  boundaries[(SIZE_XY)*res+SIZE_X*res*2+res*2] = 1;

  queue.enqueueWriteBuffer(buffer_potentials,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),potentials);
  queue.enqueueWriteBuffer(buffer_boundaries,CL_TRUE,0,sizeof(int)*(SIZE_XYZ),boundaries);


  cl::Kernel gauss_seidel(program, "jacobi");
  gauss_seidel.setArg(0, buffer_potentials);
  gauss_seidel.setArg(1, buffer_output);
  gauss_seidel.setArg(2, buffer_RHS);
  gauss_seidel.setArg(3, buffer_boundaries);
  gauss_seidel.setArg(4, SIZE_X);
  gauss_seidel.setArg(5, SIZE_Y);
  auto t1 = std::chrono::high_resolution_clock::now();

  for(int i = 0; i < 10; i++){
    queue.enqueueNDRangeKernel(gauss_seidel,cl::NullRange,cl::NDRange(SIZE_X-2,
                                                                        SIZE_Y-2,
                                                                        SIZE_Z-2),cl::NullRange);
  }
  int status = queue.finish();

  auto t2 = std::chrono::high_resolution_clock::now();

  //read result C from the device to array C
  queue.enqueueReadBuffer(buffer_potentials,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),potentials);

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()/10.0;
  std::cout << "Jacobi on " << SIZE_X <<  " took " << duration << " us, " << status << "\n";

  // display_array(potentials,SIZE_X,SIZE_Y,SIZE_Z,res);

  delete[] potentials;
  delete[] boundaries;
}


TEST(MG_GPU_OPERATORS, multires_jacobi_test)
{
  //test on non-square array
  cl::Buffer buffer_potentials(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ));
  cl::Buffer buffer_boundaries(context,CL_MEM_READ_WRITE,sizeof(int)*(SIZE_XYZ));
  cl::Buffer buffer_RHS(context,CL_MEM_READ_WRITE,sizeof(int)*(SIZE_XYZ));
  cl::Buffer buffer_output(context,CL_MEM_READ_WRITE,sizeof(int)*(SIZE_XYZ));

  float * potentials = new float[(SIZE_XYZ)];
  int * boundaries = new int[(SIZE_XYZ)];
  memset (potentials, 0, sizeof (float) * SIZE_XYZ); //zero out arrays
  memset (boundaries, 0, sizeof (int) * SIZE_XYZ);

  int res = 1;

  potentials[(SIZE_XY)*res+SIZE_X*res*2+res*2] = 200;
  boundaries[(SIZE_XY)*res+SIZE_X*res*2+res*2] = 1;

  queue.enqueueWriteBuffer(buffer_potentials,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),potentials);
  queue.enqueueWriteBuffer(buffer_boundaries,CL_TRUE,0,sizeof(int)*(SIZE_XYZ),boundaries);


  cl::Kernel gauss_seidel(program, "multires_jacobi");
  gauss_seidel.setArg(0, buffer_potentials);
  gauss_seidel.setArg(1, buffer_output);
  gauss_seidel.setArg(2, buffer_RHS);
  gauss_seidel.setArg(3, buffer_boundaries);
  gauss_seidel.setArg(4, res);
  gauss_seidel.setArg(5, SIZE_X);
  gauss_seidel.setArg(6, SIZE_Y);
  auto t1 = std::chrono::high_resolution_clock::now();

  for(int i = 0; i < 100; i++){
    queue.enqueueNDRangeKernel(gauss_seidel,cl::NullRange,cl::NDRange(SIZE_X-2,
                                                                        SIZE_Y-2,
                                                                        SIZE_Z-2),cl::NullRange);
  }
  int status = queue.finish();

  auto t2 = std::chrono::high_resolution_clock::now();

  //read result C from the device to array C
  queue.enqueueReadBuffer(buffer_potentials,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),potentials);

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()/100.0;
  std::cout << "Jacobi on " << SIZE_X <<  " took " << duration << " us, " << status << "\n";

  // display_array(potentials,SIZE_X,SIZE_Y,SIZE_Z,res);

  delete[] potentials;
  delete[] boundaries;
}

TEST(MG_GPU_OPERATORS, alloc)
{
  int size = 2000e6 / 4;
  cl::Buffer buffer_potentials(context,CL_MEM_READ_WRITE,sizeof(float)*(size));
  float * potentials = new float[size];
  memset (potentials, 0, sizeof (float) * size); //zero out arrays

  dbg(queue.enqueueWriteBuffer(buffer_potentials,CL_TRUE,0,sizeof(float)*(size),potentials));
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
//   /* -----------------------------------------------------------------------------
//   GPU buffer allocation
//   ----------------------------------------------------------------------------- */
//   cl::Buffer buffer_U(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ)); //potential approximation
//   cl::Buffer buffer_corrections(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ)); //potential approximation
//   cl::Buffer buffer_U1(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ)); //potential approximation
//   cl::Buffer buffer_R(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ)); //potential approximation
//   cl::Buffer buffer_output(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ)); //boundary condition values
//   cl::Buffer buffer_b(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ)); //boundary condition values
//   cl::Buffer buffer_F(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ)); //potential approximation
//   /* -----------------------------------------------------------------------------
//   GPU kernel initialization
//   ----------------------------------------------------------------------------- */
//   cl::Kernel unigrid(program, "compute_unigrid_corrections");
//   unigrid.setArg(0, buffer_U);
//   unigrid.setArg(1, buffer_corrections);
//   unigrid.setArg(2, buffer_F);
//   unigrid.setArg(3, buffer_b);
//   unigrid.setArg(4, 1);
//   unigrid.setArg(5, SIZE_X);
//   unigrid.setArg(6, SIZE_Y);
//   unigrid.setArg(7, SIZE_Z);
//   /* -----------------------------------------------------------------------------
//   Local buffer allocation
//   ----------------------------------------------------------------------------- */
//   float * U = new float[(SIZE_XYZ)];
//   float * F = new float[(SIZE_XYZ)];
//   float * b = new float[(SIZE_XYZ)];
//   memset (U, 0, sizeof (float) * SIZE_XYZ); //zero out arrays
//   memset (F, 0, sizeof (float) * SIZE_XYZ); //zero out arrays
//   memset (b, 0, sizeof (float) * SIZE_XYZ);
//   /* -----------------------------------------------------------------------------
//   Demo problem setup
//   ----------------------------------------------------------------------------- */
//   for(int x = 40; x < 50; x++){
//     for(int y = 40; y < 50; y++){
//       U[(SIZE_XY*8)+SIZE_X*y+x] = 1;
//       b[(SIZE_XY*8)+SIZE_X*y+x] = 1;
//     }
//   }
//   /* -----------------------------------------------------------------------------
//   Copy problem to GPU memory
//   ----------------------------------------------------------------------------- */
//   queue.enqueueWriteBuffer(buffer_U,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),U);
//   queue.enqueueWriteBuffer(buffer_F,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),F);
//   queue.enqueueWriteBuffer(buffer_b,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),b);
//   /* -----------------------------------------------------------------------------
//   Max stores the greatest value in the residual; this is cheaper than finding the norm
//   ----------------------------------------------------------------------------- */
//   // double max = 0;
//
//   const int MG_CYCLES = 10;
//
//   // float previous_norm = 1;
//   // float convergence_factor = 1;
//
//   auto t1 = std::chrono::high_resolution_clock::now();
//   for(int cycles = 0; cycles < MG_CYCLES; cycles++){
//     // for(int level = 5; level > -1; level--){
//       int step_size = pow(2,0);
//       unigrid.setArg(4, step_size);
//       queue.enqueueNDRangeKernel(unigrid,cl::NullRange,cl::NDRange(SIZE_X-2*step_size,
//                                                                           SIZE_Y-2*step_size,
//                                                                           SIZE_Z-2*step_size),cl::NullRange);
//
//       // queue.enqueueCopyBuffer(buffer_output,buffer_U,0,0,sizeof(float)*(SIZE_XYZ));
//       //copy residuals - double buffer would speed up
//
//     // }
//   }
//   int status = queue.finish();
//
//
//   auto t2 = std::chrono::high_resolution_clock::now();
//
//   printf("s%i\n",status);
//
//
//   queue.enqueueReadBuffer(buffer_U,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),U);
//   queue.finish();
//   display_array(U,SIZE_X,SIZE_Y,SIZE_Z,8);
//
//   auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()/10.0;
//   std::cout << "Multigrid on " << SIZE_X <<  "^3 took " << duration << " us\n";
//
//
//   delete[] U;
//   delete[] F;
//   delete[] b;
// }





TEST(MG_GPU_OPERATORS, full_up_test)
{


  //test on non-square array
  cl::Buffer buffer_potentials(context,CL_MEM_READ_WRITE,sizeof(float)*(SIZE_XYZ));
  cl::Buffer buffer_boundaries(context,CL_MEM_READ_WRITE,sizeof(int)*(SIZE_XYZ));
  cl::Buffer buffer_RHS(context,CL_MEM_READ_WRITE,sizeof(int)*(SIZE_XYZ));
  cl::Buffer buffer_output(context,CL_MEM_READ_WRITE,sizeof(int)*(SIZE_XYZ));

  float * potentials = new float[(SIZE_XYZ)];
  int * boundaries = new int[(SIZE_XYZ)];
  memset (potentials, 0, sizeof (float) * SIZE_XYZ); //zero out arrays
  memset (boundaries, 0, sizeof (int) * SIZE_XYZ);

  int res = 1;

  potentials[(SIZE_XY)*res+SIZE_X*res*2+res*2] = 200;
  boundaries[(SIZE_XY)*res+SIZE_X*res*2+res*2] = 1;

  queue.enqueueWriteBuffer(buffer_potentials,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),potentials);
  queue.enqueueWriteBuffer(buffer_boundaries,CL_TRUE,0,sizeof(int)*(SIZE_XYZ),boundaries);


  cl::Kernel gauss_seidel(program, "jacobi");
  gauss_seidel.setArg(0, buffer_potentials);
  gauss_seidel.setArg(1, buffer_output);
  gauss_seidel.setArg(2, buffer_RHS);
  gauss_seidel.setArg(3, buffer_boundaries);
  gauss_seidel.setArg(4, SIZE_X);
  gauss_seidel.setArg(5, SIZE_Y);
  auto t1 = std::chrono::high_resolution_clock::now();

  for(int i = 0; i < 10; i++){
    queue.enqueueNDRangeKernel(gauss_seidel,cl::NullRange,cl::NDRange(SIZE_X-2,
                                                                        SIZE_Y-2,
                                                                        SIZE_Z-2),cl::NullRange);
  }
  int status = queue.finish();

  auto t2 = std::chrono::high_resolution_clock::now();

  //read result C from the device to array C
  queue.enqueueReadBuffer(buffer_potentials,CL_TRUE,0,sizeof(float)*(SIZE_XYZ),potentials);

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()/10.0;
  std::cout << "Jacobi on " << SIZE_X <<  " took " << duration << " us, " << status << "\n";

  // display_array(potentials,SIZE_X,SIZE_Y,SIZE_Z,res);

  delete[] potentials;
  delete[] boundaries;
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
