find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(MPI REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ../src/ ${MPI_INCLUDE_PATH})


add_executable (multiphysics_test multiphysics_test.cpp)

link_directories(../src/)

target_link_libraries(multiphysics_test ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} multiphysics_lib ${MPI_LIBRARIES})

target_link_libraries(multiphysics_test CppUTest )
target_link_libraries(multiphysics_test CppUTestExt )

target_compile_options(multiphysics_test PRIVATE -Wall -Wextra -pedantic -std=c++17 -Werror -O3)

if(VTK_LIBRARIES)
  target_link_libraries(multiphysics_test ${VTK_LIBRARIES})
else()
  target_link_libraries(multiphysics_test vtkHybrid vtkWidgets)
endif()

add_test (NAME test COMMAND multiphysics_test)
