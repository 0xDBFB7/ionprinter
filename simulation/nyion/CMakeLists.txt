#export ASAN_OPTIONS=protect_shadow_gap=0:replace_intrin=0
# and possibly detect_leaks=0
# must be set prior to running this program.
# these flags may introduce strange bugs. The new dynamic shadow feature of ASAN might fix this.


cmake_minimum_required(VERSION 3.9)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CUDA_COMPILER /usr/local/cuda-10.2/bin/nvcc)

set (GTEST_DONT_DEFINE_TEST 1)

set(CUDA_NVCC_FLAGS -std=c++11 -g -Xcompiler -fexceptions -Xcompiler -fPIC -O3 -Xptxas -O3,-v)
#one -O3 for host, one for device.
#-v verbose mode - thanks Luca Ferraro!

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror -fopenmp\
 -foffload=-lm -Wno-unused-parameter -fno-omit-frame-pointer  -fsanitize=address -fsanitize=undefined -fsanitize=leak -g --coverage")


#Turbo Boost: -ffast-math on host, -use_fast_math nvcc

set(CUDA_PROPAGATE_HOST_FLAGS OFF)

set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined -fsanitize=leak -g")
#apparently not good cmake. Works, however.

PROJECT(nyion LANGUAGES CUDA CXX C)


set(FLTK_DIR ~/Programs/fltk-1.3.5/build/)

find_package(FLTK REQUIRED NO_MODULE)
find_package(OpenMP REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(MathGL2 REQUIRED)
find_package(CUDA REQUIRED)

### Note that FLTK's OpenGL support has been disabled. Probably can be re-enabled.
include_directories(/home/arthurdent/Programs/fltk-1.3.5/build/ /home/arthurdent/Programs/fltk-1.3.5/FL ${CMAKE_SOURCE_DIR} ${MATHGL2_INCLUDE_DIR} ${OPENGL_INCLUDE_DIRS}
                                                        ${GLUT_INCLUDE_DIRS} ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES} ../src/)

set(CMAKE_BUILD_TYPE Debug)
add_subdirectory (src)

enable_testing ()
add_subdirectory (test)




configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)
