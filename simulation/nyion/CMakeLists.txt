# export ASAN_OPTIONS=protect_shadow_gap=0:replace_intrin=0
# or ASAN_OPTIONS=protect_shadow_gap=0:replace_intrin=0:halt_on_error=0:verbosity=1
# and possibly detect_leaks=0
# must be set prior to running this program.
# If you get strange garbage from cuda code, this probably broke.
# these flags may introduce strange bugs. The new dynamic shadow feature of ASAN might fix this.
# const char * __asan_default_options() would be a better solution; but it doesn't seem to work in this version of GCC.
if(NOT DEFINED ENV{ASAN_OPTIONS})
    message( FATAL_ERROR "oh no, ASAN_OPTIONS isn't set! This is zero cool.")
endif()


cmake_minimum_required(VERSION 3.9)
set (CMAKE_CXX_STANDARD 17)

set(CMAKE_CUDA_COMPILER /usr/local/cuda-10.2/bin/nvcc)

set(CMAKE_CUDA_SEPARABLE_COMPILATION ON)
set(CMAKE_CUDA_RESOLVE_DEVICE_SYMBOLS ON)


set(CUDA_NVCC_FLAGS "-arch=sm_61 -std=c++11 -g -Xcompiler -fexceptions -Xcompiler -fPIC -O3 -Xptxas -O3,-v")
#one -O3 for host, one for device. Set -arch appropriately for your GPU.
#-v verbose mode - thanks Luca Ferraro!
# --generate-line-info?
# --compiler-options -Wall -Wextra -pedantic -Werror --resource-usage?
# -lineinfo, -G
# -rdynamic on host
# for more useful cuda-memcheck
# also, cuobjdump --dump-sass

set(CUDA_PROPAGATE_HOST_FLAGS OFF)


set(CXX_ASAN_FLAGS "-fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined -fsanitize=leak ")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror -fopenmp\
 -foffload=-lm -Wno-unused-parameter ${CXX_ASAN_FLAGS} -g --coverage")

#Turbo Boost: -ffast-math on host, -use_fast_math nvcc

set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} ${CXX_ASAN_FLAGS} -g")


PROJECT(nyion LANGUAGES CUDA CXX C)


set(FLTK_DIR ~/Programs/fltk-1.3.5/build/)
set(nlohmann_json_DIR /home/arthurdent/Programs/json/build/)
find_package(FLTK REQUIRED NO_MODULE)
find_package(OpenMP REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(MathGL2 REQUIRED)
find_package(CUDA REQUIRED)
find_package(nlohmann_json REQUIRED)

### Note that FLTK's OpenGL support has been disabled. Probably can be re-enabled.
include_directories(${CMAKE_SOURCE_DIR} ${MATHGL2_INCLUDE_DIR} ${OPENGL_INCLUDE_DIRS}
                                                        ${GLUT_INCLUDE_DIRS} ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES} ../src/)


include_directories(/home/arthurdent/Programs/fltk-1.3.5/build/ /home/arthurdent/Programs/fltk-1.3.5/FL nlohmann_json::nlohmann_json)

include_directories(/home/arthurdent/Programs/cpp-linenoise)
# from https://github.com/yhirose/cpp-linenoise/commits/master, version 5ea11d5d97e1f770524c300dd67db009dd4a3ab4
#include_directories(/home/arthurdent/Programs/nlohmann-json/)
# from https://github.com/nlohmann/json, version 3.7.3



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
