find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

add_executable (multiphysics_test multiphysics_test.cpp)

target_link_libraries(multiphysics_test ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

if(VTK_LIBRARIES)
target_link_libraries(multiphysics_test ${VTK_LIBRARIES})
else()
  target_link_libraries(multiphysics_test vtkHybrid vtkWidgets)
endif()

add_test (NAME test COMMAND multiphysics_test)
