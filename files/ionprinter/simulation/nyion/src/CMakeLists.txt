find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

add_executable(multiphysics main.cpp)

add_library (multiphysics_lib multiphysics.cpp multiphysics.hpp)

target_compile_options(multiphysics PRIVATE -Wall -Wextra -pedantic -Werror -O0)

if(VTK_LIBRARIES)
  target_link_libraries(multiphysics ${VTK_LIBRARIES} multiphysics_lib)
else()
  target_link_libraries(multiphysics vtkHybrid vtkWidgets multiphysics_lib)
endif()
