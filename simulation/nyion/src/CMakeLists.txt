add_library (nyion_lib nyion.cpp nyion.hpp)

add_executable(nyion nyion.cpp)

target_link_libraries(nyion ${ROOT_LIBRARIES})
target_link_libraries(nyion_lib ${ROOT_LIBRARIES})
