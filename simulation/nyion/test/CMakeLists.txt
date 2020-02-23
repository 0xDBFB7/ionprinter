include_directories(../src/)
link_directories(../src/)

include_directories(../src/common/ ../src/physics_mesh ../src/array_helper_program/ ../src/visualize/ ../src/device_data_structure/)
link_directories(../src/common/ ../src/physics_mesh ../src/array_helper_program/ ../src/visualize/ ../src/device_data_structure/)


add_executable (nyion_test nyion_test.cpp)
target_link_libraries(nyion_test ${MATHGL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CUDA_LIBRARIES}
                                    nyion_lib physics_mesh physics_mesh_transfers visualize common OpenMP::OpenMP_CXX)

target_link_libraries(nyion_test state_persist)

target_link_libraries(nyion_test gtest gtest_main )


target_compile_options(nyion_test PRIVATE)

add_test (NAME test COMMAND nyion_test)
