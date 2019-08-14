find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(OpenCL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(MPI REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${MPI_INCLUDE_PATH} ${OPENCL_INCLUDE_DIR})

add_executable(nyion main.cpp)

add_library (nyion_lib
                        nyion.cpp nyion.hpp)

target_compile_options(nyion PRIVATE -Wall -Wextra -pedantic -Werror -std=c++17 -O3)


if(VTK_LIBRARIES)
  target_link_libraries(nyion ${VTK_LIBRARIES} nyion_lib ${MPI_LIBRARIES} ${OpenCL_LIBRARY})
else()
  target_link_libraries(nyion vtkHybrid vtkWidgets nyion_lib ${MPI_LIBRARIES} ${OpenCL_LIBRARY})
endif()
