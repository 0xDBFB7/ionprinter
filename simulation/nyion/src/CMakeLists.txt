include_directories(debug_helpers/ host_data_structure/ array_helper_program/ physics/ visualize/ device_data_structure/)

FILE(GLOB children RELATIVE ${CMAKE_SOURCE_DIR}/src/ ${CMAKE_SOURCE_DIR}/src/*)
foreach(dir ${children})
  link_directories(${dir})
  add_subdirectory(${dir})
endforeach()


add_library (nyion_lib nyion.cpp nyion.hpp)
add_library (data_structure host_data_structure/host_data_structure.cpp data_structure.hpp)
add_library (visualize visualize.cpp visualize.hpp)
add_library (debug_helpers debug_helpers.cpp debug_helpers.hpp)
add_library (unrolled_operations unrolled_operations.cu CUDA_helpers.cu)

add_executable(nyion main.cpp)
add_executable(array_helper array_helper_program/array_helper.cpp)

set_target_properties(unrolled_operations PROPERTIES CUDA_SEPARABLE_COMPILATION ON)


target_link_libraries(nyion ${MATHGL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CUDA_LIBRARIES} nyion_lib data_structure visualize debug_helpers OpenMP::OpenMP_CXX )
target_link_libraries(array_helper ${MATHGL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CUDA_LIBRARIES} nyion_lib data_structure visualize debug_helpers OpenMP::OpenMP_CXX )
target_link_libraries(data_structure ${MATHGL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CUDA_LIBRARIES} nyion_lib visualize debug_helpers OpenMP::OpenMP_CXX )

target_link_libraries(unrolled_operations ${MATHGL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CUDA_LIBRARIES} nyion_lib visualize debug_helpers OpenMP::OpenMP_CXX )
