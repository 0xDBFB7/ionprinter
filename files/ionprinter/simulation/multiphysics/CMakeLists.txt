cmake_minimum_required(VERSION 2.8)

PROJECT(multiphysics)

set(CMAKE_BUILD_TYPE Debug)
add_subdirectory (src)

enable_testing ()
add_subdirectory (test)
