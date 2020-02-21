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

void CUDA_simple_copy_test();
void CUDA_simple_struct_copy_test();
void CUDA_simple_struct_copy_test_2();
void CUDA_struct_copy_test();
void CUDA_physics_mesh_copy_test();


//
// TEST(CUDA, CUDA_simple_copy_test){
//     CUDA_simple_copy_test();
// }
//
// TEST(CUDA, CUDA_simple_struct_copy_test){
//     CUDA_simple_struct_copy_test();
// }
//
// TEST(CUDA, CUDA_simple_struct_copy_test_2){
//     CUDA_simple_struct_copy_test_2();
// }

TEST(CUDA, CUDA_physics_mesh_copy_test){
    CUDA_physics_mesh_copy_test();
    //ensure unchanged!
}
