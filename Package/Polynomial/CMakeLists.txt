include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. POLYNOMIAL_SRCS)
add_library(Polynomial MODULE ${POLYNOMIAL_SRCS})

