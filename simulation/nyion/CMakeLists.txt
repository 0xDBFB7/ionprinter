cmake_minimum_required(VERSION 3.1)

set(CMAKE_C_COMPILER ~/Programs/gcc-offload/gcc-offload/install/bin/gcc PATH "")
set(CMAKE_CXX_COMPILER ~/Programs/gcc-offload/gcc-offload/install/bin/g++ CACHE PATH "")
set (CMAKE_CXX_STANDARD 17)

PROJECT(nyion)

find_package(ROOT REQUIRED COMPONENTS RooFitCore MathCore RIO Hist Tree Net)
find_package(OpenMP REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_definitions(${ROOT_CXX_FLAGS})

include(${ROOT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ../src/)

add_compile_options(-Wall -Wextra -pedantic -Werror -std=c++17 -O3 -fopenmp -foffload=-lm)


list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
#---Create  a main program using the library


set(CMAKE_BUILD_TYPE Debug)
add_subdirectory (src)

enable_testing ()
add_subdirectory (test)
