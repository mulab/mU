include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. POLYNOMIAL_SRCS)
add_library(Polynomial ${PACKAGE_LIB_TYPE} ${POLYNOMIAL_SRCS})
target_link_libraries(Polynomial mU gmp)
