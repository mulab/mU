cmake_minimum_required(VERSION 2.6)

project(mU0Link CXX)

include_directories(${mU0Link_SOURCE_DIR}/../../Include)
link_directories(${mU0Link_SOURCE_DIR}/../.. ${mU0Link_SOURCE_DIR}/../../Lib)
#find_package(mU0)
include(mU0Config.cmake)

add_subdirectory(Bind)
add_subdirectory(Embed)

AUX_SOURCE_DIRECTORY(. MU0LINK_SRCS)
add_library(mu0link SHARED ${MU0LINK_SRCS})
target_link_libraries(mu0link kernel mU0)