
add_library (nyion_lib nyion.cpp nyion.hpp)
add_library (data_structure data_structure.cpp data_structure.hpp)
add_library (visualize visualize.cpp visualize.hpp)
add_library (debug_helpers debug_helpers.cpp debug_helpers.hpp)

add_executable(nyion main.cpp)
add_executable(array_helper array_helper.cpp)

target_link_libraries(nyion ${MATHGL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} nyion_lib data_structure visualize debug_helpers OpenMP::OpenMP_CXX )

target_link_libraries(array_helper ${MATHGL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} nyion_lib data_structure visualize debug_helpers OpenMP::OpenMP_CXX )
target_link_libraries(data_structure ${MATHGL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} nyion_lib visualize debug_helpers OpenMP::OpenMP_CXX )
