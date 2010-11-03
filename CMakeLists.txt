cmake_minimum_required(VERSION 2.6)

project(mU CXX)

set(mU_HEADER_DIR ${mU_SOURCE_DIR}/Include /usr/include /usr/include/c++/4.4)
set(CMAKE_CXX_FLAGS "-std=c++0x -L.")
set(EXECUTABLE_OUTPUT_PATH ..)
set(LIBRARY_OUTPUT_PATH ..)

add_subdirectory(Kernel)
add_subdirectory(Parser)
add_subdirectory(System)
add_subdirectory(Interface)
add_subdirectory(String)
add_subdirectory(Number)
add_subdirectory(Function)
add_subdirectory(mU)
