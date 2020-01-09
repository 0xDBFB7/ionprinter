

include_directories(../src/)
link_directories(../src/)

add_executable (nyion_test nyion_test.cpp)
target_link_libraries(nyion_test ${MATHGL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CUDA_LIBRARIES}
                                    nyion_lib data_structure visualize debug_helpers unrolled_operations OpenMP::OpenMP_CXX )

set_target_properties(nyion_test PROPERTIES CUDA_SEPARABLE_COMPILATION ON)


target_link_libraries(nyion_test CppUTest )
target_link_libraries(nyion_test CppUTestExt )

target_compile_options(nyion_test PRIVATE)

add_test (NAME test COMMAND nyion_test)
