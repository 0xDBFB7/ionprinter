
add_library (nyion_lib nyion.cpp nyion.hpp)
add_library (data_structure data_structure.cpp data_structure.hpp)
add_library (visualize visualize.cpp visualize.hpp)

add_executable(nyion main.cpp)
add_executable(array_helper array_helper.cpp)

target_link_libraries(nyion data_structure visualize ${ROOT_LIBRARIES})
