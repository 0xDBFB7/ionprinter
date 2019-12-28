
include_directories(../src/)
link_directories(../src/)

add_executable (nyion_test nyion_test.cpp)
target_link_libraries(nyion_test ${ROOT_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} nyion_lib OpenMP::OpenMP_CXX)

target_link_libraries(nyion_test CppUTest )
target_link_libraries(nyion_test CppUTestExt )

target_compile_options(nyion_test PRIVATE)

add_test (NAME test COMMAND nyion_test)
