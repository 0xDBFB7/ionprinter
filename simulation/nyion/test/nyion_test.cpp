#include "nyion.hpp"
// #include "visualize.hpp"
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"

TEST_GROUP(MG_GPU_OPERATORS)
{
};

TEST(MG_GPU_OPERATORS, test)
{
}




int main(int ac, char** av){
  std::cout << "Number of devices: " << omp_get_num_devices() << std::endl;
  std::cout << "Default device: " << omp_get_default_device() << std::endl;

  bool offload;
  #pragma omp target defaultmap(tofrom:scalar)
  offload = !omp_is_initial_device();

  if (offload) {
    printf("Able to use offloading!\n");
  }



  return RUN_ALL_TESTS(ac, av);
}
