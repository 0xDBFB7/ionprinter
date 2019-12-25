
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})

add_executable(nyion main.cpp)

add_library (nyion_lib nyion.cpp nyion.hpp)

target_compile_options(nyion PRIVATE)

target_link_libraries(nyion nyion_lib ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} nyion_lib ROOT::Physics OpenMP::OpenMP_CXX)
