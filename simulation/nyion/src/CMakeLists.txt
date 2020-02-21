include_directories(debug_helpers/ host_data_structure/ array_helper_program/ physics/ visualize/ device_data_structure/ ./)
link_directories(debug_helpers/ host_data_structure/ array_helper_program/ physics/ visualize/ device_data_structure/ ./)


add_library (nyion_lib nyion.cpp nyion.hpp)
add_library (data_structure host_data_structure/physics_mesh.cpp host_data_structure/traverse_state.cpp host_data_structure/host_data_structure.hpp)
add_library (visualize visualize/visualize.cpp visualize/visualize.hpp)
add_library (debug_helpers debug_helpers/debug_helpers.cpp debug_helpers/debug_helpers.hpp)
add_library (device_data_structure device_data_structure/device_data_structure.cu device_data_structure/device_data_structure_test.cu debug_helpers/CUDA_helpers.cu)
add_library (test_persistence test_persistence/store_tests.cpp test_persistence/store_tests.hpp)

add_executable(nyion main.cpp)
add_executable(array_helper array_helper_program/array_helper.cpp)

set_target_properties(device_data_structure PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

target_link_libraries(nyion ${MATHGL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CUDA_LIBRARIES} nyion_lib data_structure visualize debug_helpers OpenMP::OpenMP_CXX )


target_link_libraries(array_helper ${MATHGL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CUDA_LIBRARIES} OpenMP::OpenMP_CXX )
target_link_libraries(array_helper fltk nyion_lib data_structure test_persistence visualize debug_helpers)

target_link_libraries(visualize ${MATHGL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CUDA_LIBRARIES} nyion_lib data_structure debug_helpers OpenMP::OpenMP_CXX )

target_link_libraries(data_structure nyion_lib)

target_link_libraries(device_data_structure ${MATHGL2_LIBRARY} ${CUDA_LIBRARIES} debug_helpers OpenMP::OpenMP_CXX gtest gtest_main )
