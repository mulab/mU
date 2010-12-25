cmake_minimum_required(VERSION 2.6)

project(EclLink CXX)

include_directories(${EclLink_SOURCE_DIR}/../../Include)
link_directories(${EclLink_SOURCE_DIR}/../.. ${EclLink_SOURCE_DIR}/../../Lib)
#find_package(ecl)
include(eclConfig.cmake)

add_subdirectory(Embed)

AUX_SOURCE_DIRECTORY(. ECLLINK_SRCS)
add_library(ecllink SHARED ${ECLLINK_SRCS})
target_link_libraries(ecllink kernel ecl)
