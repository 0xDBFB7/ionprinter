include_directories(debug_helpers/ host_data_structure/ array_helper_program/ physics/ visualize/ device_data_structure/ ./)
link_directories(debug_helpers/ host_data_structure/ array_helper_program/ physics/ visualize/ device_data_structure/ ./)


add_library (nyion_lib nyion.cpp nyion.hpp)
add_library (data_structure host_data_structure/host_data_structure.cpp host_data_structure/host_data_structure.hpp)
add_library (visualize visualize/visualize.cpp visualize/visualize.hpp)
add_library (debug_helpers debug_helpers/debug_helpers.cpp debug_helpers/debug_helpers.hpp)
add_library (unrolled_operations device_data_structure/unrolled_operations.cu debug_helpers/CUDA_helpers.cu)

add_executable(nyion main.cpp)
add_executable(array_helper array_helper_program/array_helper.cpp)

set_target_properties(unrolled_operations PROPERTIES CUDA_SEPARABLE_COMPILATION ON)


target_link_libraries(nyion ${MATHGL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CUDA_LIBRARIES} nyion_lib data_structure visualize debug_helpers OpenMP::OpenMP_CXX )
target_link_libraries(array_helper ${MATHGL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CUDA_LIBRARIES} fltk nyion_lib data_structure visualize debug_helpers OpenMP::OpenMP_CXX )

target_link_libraries(data_structure ${MATHGL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CUDA_LIBRARIES} nyion_lib visualize debug_helpers OpenMP::OpenMP_CXX )

target_link_libraries(unrolled_operations ${MATHGL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CUDA_LIBRARIES} nyion_lib visualize debug_helpers OpenMP::OpenMP_CXX )
