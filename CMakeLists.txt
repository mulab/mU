cmake_minimum_required(VERSION 2.6)

project(mU CXX)

set(mU_HEADER_DIR ${mU_SOURCE_DIR}/Include)
set(CMAKE_CXX_FLAGS "-O3 -Wno-deprecated")
set(CMAKE_SHARED_MODULE_PREFIX "")
set(CMAKE_SHARED_MODULE_SUFFIX ".dll")
set(EXECUTABLE_OUTPUT_PATH ..)
set(LIBRARY_OUTPUT_PATH .)

add_subdirectory(Kernel)
add_subdirectory(Package)
