#ASAN_OPTIONS=protect_shadow_gap=0 must be set prior to running this program.
# this flag may introduce strange bugs. The new dynamic shadow feature of ASAN might fix this.

cmake_minimum_required(VERSION 3.9)
#set (CMAKE_CXX_STANDARD 17)

set(CMAKE_CUDA_COMPILER /usr/local/cuda-10.2/bin/nvcc)

#add_compile_options(-Wall -Wextra -pedantic -Werror -std=c++17 -fopenmp -foffload=-lm -Wno-unused-parameter)

#set(CMAKE_C_COMPILER ~/Programs/gcc-offload/gcc-offload/install/bin/gcc CACHE PATH "")
#set(CMAKE_CXX_COMPILER ~/Programs/gcc-offload/gcc-offload/install/bin/g++ CACHE PATH "")


set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror -fopenmp\
 -foffload=-lm -Wno-unused-parameter -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined -fsanitize=leak -g")

set(CUDA_PROPAGATE_HOST_FLAGS OFF)
#set(CUDA_HOST_COMPILATION_CPP ON)
set(CUDA_NVCC_FLAGS -std=c++11 -g -Xcompiler -fexceptions -Xcompiler -fPIC)
#set(CMAKE_C_FLAGS "-g -fPIC")
#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-g -fPIC")
set(CUDA_SEPARABLE_COMPILATION ON)

set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined -fsanitize=leak -g")
#apparently not good cmake. Works, however.


message(${CUDA_NVCC_FLAGS})

PROJECT(nyion LANGUAGES CUDA CXX C)


find_package(OpenMP REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(MathGL2 REQUIRED)
find_package(CUDA REQUIRED)


include_directories(${CMAKE_SOURCE_DIR} ${MATHGL2_INCLUDE_DIR} $(CPPUTEST_HOME)/include ${OPENGL_INCLUDE_DIRS}
                                                        ${GLUT_INCLUDE_DIRS} ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES} ../src/)

set(CMAKE_BUILD_TYPE Debug)
add_subdirectory (src)

enable_testing ()
add_subdirectory (test)
