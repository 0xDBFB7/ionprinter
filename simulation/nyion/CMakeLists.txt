cmake_minimum_required(VERSION 3.1)
#set (CMAKE_CXX_STANDARD 17)

set(CMAKE_CUDA_COMPILER /usr/local/cuda-10.2/bin/nvcc)

add_compile_options(-Wall -Wextra -pedantic -Werror -std=c++17 -g  -fopenmp -foffload=-lm -Wno-unused-parameter)

#set(CMAKE_C_COMPILER ~/Programs/gcc-offload/gcc-offload/install/bin/gcc CACHE PATH "")
#set(CMAKE_CXX_COMPILER ~/Programs/gcc-offload/gcc-offload/install/bin/g++ CACHE PATH "")


PROJECT(nyion LANGUAGES C CXX CUDA)

find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net)
find_package(OpenMP REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(MathGL2 REQUIRED)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

add_definitions(${ROOT_CXX_FLAGS})

include(${ROOT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR} ${MATHGL2_INCLUDE_DIR} $(CPPUTEST_HOME)/include ${ROOT_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ../src/)

set(CMAKE_BUILD_TYPE Debug)
add_subdirectory (src)

enable_testing ()
add_subdirectory (test)
