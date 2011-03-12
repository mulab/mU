cmake_minimum_required(VERSION 2.6)

project(mU CXX)

set(mU_HEADER_DIR ${mU_SOURCE_DIR}/Include)
#set(CMAKE_CXX_FLAGS "-std=c++0x -Wl,--enable-auto-import -L.")
set(CMAKE_CXX_FLAGS "-std=c++0x -L.")
set(EXECUTABLE_OUTPUT_PATH ../bin)
set(LIBRARY_OUTPUT_PATH ../bin)

add_subdirectory(Combinatorics)
add_subdirectory(Kernel)
add_subdirectory(mU)
add_subdirectory(Number)
add_subdirectory(Numeric)
add_subdirectory(Parser)
add_subdirectory(Polynomial)
add_subdirectory(System)
add_subdirectory(String)
