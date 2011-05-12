include(${CMAKE_SOURCE_DIR}/UnifyPackageNameFormat.cmake)
include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. GRAPHICS_SRCS)

add_library(Graphics ${PACKAGE_LIB_TYPE} ${GRAPHICS_SRCS})

target_link_libraries(Graphics Kernel gmp)
