include_directories(debug_helpers/ physics_mesh/ array_helper_program/ visualize/ device_data_structure/ ./)

link_directories(./ debug_helpers/ physics_mesh/ array_helper_program/ visualize/ device_data_structure/)




add_library (physics_mesh
                    physics_mesh/physics_mesh.cpp physics_mesh/traverse_state.cpp
                    physics_mesh/device_transfer.cu physics_mesh/device_transfer_tests.cu)

set_target_properties(physics_mesh PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
target_link_libraries(physics_mesh ${CUDA_LIBRARIES} device_data_structure gtest gtest_main)


add_library (visualize visualize/visualize.cpp visualize/visualize.hpp)
target_link_libraries(visualize ${MATHGL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} physics_mesh debug_helpers)


add_library (debug_helpers debug_helpers/CUDA_helpers.cu debug_helpers/debug_helpers.cpp debug_helpers/debug_helpers.hpp)
target_link_libraries(debug_helpers gtest gtest_main)
set_target_properties(debug_helpers PROPERTIES CUDA_SEPARABLE_COMPILATION ON)


add_library (state_persist state_persist/state_persist.cpp)


add_library (device_data_structure device_data_structure/struct_transfer_helpers.cu device_data_structure/struct_transfer_helpers.hpp)
set_target_properties(device_data_structure PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
target_link_libraries(device_data_structure ${CUDA_LIBRARIES} debug_helpers)



add_executable(nyion main.cpp)
target_link_libraries(nyion ${MATHGL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CUDA_LIBRARIES} nyion_lib gtest gtest_main visualize debug_helpers OpenMP::OpenMP_CXX )


add_executable(array_helper array_helper_program/array_helper.cpp)
target_link_libraries(array_helper ${MATHGL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CUDA_LIBRARIES} OpenMP::OpenMP_CXX )
target_link_libraries(array_helper fltk nyion_lib physics_mesh state_persist visualize debug_helpers)


add_library (nyion_lib nyion.cpp nyion.hpp)
