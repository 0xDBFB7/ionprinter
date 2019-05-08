cmake_minimum_required(VERSION 2.8)

PROJECT(multiphysics)

set(CMAKE_BUILD_TYPE Debug)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

add_executable(multiphysics MACOSX_BUNDLE multiphysics.cpp)

target_link_libraries(multiphysics ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )


if(VTK_LIBRARIES)
  target_link_libraries(multiphysics ${VTK_LIBRARIES})
else()
  target_link_libraries(multiphysics vtkHybrid vtkWidgets)
endif()
