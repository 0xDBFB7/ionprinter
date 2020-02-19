#include "nyion.hpp"
#include "visualize.hpp"
#include "host_data_structure.hpp"
#include "device_data_structure.hpp"

#include "host_data_structure_test.hpp"


#include "gtest/gtest.h"


TEST(CUDA, CUDA_display_info)
{
  display_GPU_info();
}

void CUDA_struct_copy_test();


TEST(CUDA, CUDA_struct_copy_test){
    CUDA_struct_copy_test();
}
