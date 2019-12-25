
add_executable(nyion main.cpp)

add_library (nyion_lib nyion.cpp nyion.hpp)

target_compile_options(nyion PRIVATE)

target_link_libraries(nyion nyion_lib ${ROOT_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} nyion_lib OpenMP::OpenMP_CXX)
