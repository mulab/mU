include(${CMAKE_SOURCE_DIR}/UnifyPackageNameFormat.cmake)
include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. NUMBERTHEORY_SRCS)

add_library(NumberTheory ${PACKAGE_LIB_TYPE} ${NUMBERTHEORY_SRCS})

target_link_libraries(NumberTheory Kernel gmp)

