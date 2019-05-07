cmake_minimum_required(VERSION 2.8)

PROJECT(multiphysics)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(multiphysics MACOSX_BUNDLE multiphysics.cpp)

if(VTK_LIBRARIES)
  target_link_libraries(multiphysics ${VTK_LIBRARIES})
else()
  target_link_libraries(multiphysics vtkHybrid vtkWidgets)
endif()

