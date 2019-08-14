cmake_minimum_required(VERSION 2.8)

PROJECT(nyion)

add_compile_options(-Wall -Wextra -pedantic -Werror -std=c++17 -O3)

set(CMAKE_BUILD_TYPE Debug)
add_subdirectory (src)

enable_testing ()
add_subdirectory (test)
