include_directories(../src/)
link_directories(../src/)

include_directories(../src/debug_helpers/ ../src/host_data_structure/ ../src/array_helper_program/ ../src/physics/ ../src/visualize/ ../src/device_data_structure/)
link_directories(../src/debug_helpers/ ../src/host_data_structure/ ../src/array_helper_program/ ../src/physics/ ../src/visualize/ ../src/device_data_structure/)


add_executable (nyion_test nyion_test.cpp)
target_link_libraries(nyion_test ${MATHGL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CUDA_LIBRARIES}
                                    nyion_lib data_structure visualize debug_helpers unrolled_operations OpenMP::OpenMP_CXX)


#target_link_libraries(nyion_test CppUTest )
#target_link_libraries(nyion_test CppUTestExt )
target_link_libraries(nyion_test gtest gtest_main )

target_compile_options(nyion_test PRIVATE)

add_test (NAME test COMMAND nyion_test)
