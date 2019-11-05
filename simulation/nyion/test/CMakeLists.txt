find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(OpenCL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(MPI REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ../src/ ${MPI_INCLUDE_PATH} ${OPENCL_INCLUDE_DIR})


add_executable (nyion_test nyion_test.cpp)

link_directories(../src/)

target_link_libraries(nyion_test ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} nyion_lib ${MPI_LIBRARIES})

target_link_libraries(nyion_test CppUTest )
target_link_libraries(nyion_test CppUTestExt )

target_compile_options(nyion_test PRIVATE -Wall -Wextra -pedantic -Werror -std=c++17 -O3)

if(VTK_LIBRARIES)
  target_link_libraries(nyion_test ${VTK_LIBRARIES} ${OpenCL_LIBRARY})
else()
  target_link_libraries(nyion_test vtkHybrid vtkWidgets ${OpenCL_LIBRARY})
endif()

add_test (NAME test COMMAND nyion_test)
