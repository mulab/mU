include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. MATRIX_SRCS)
add_library(Matrix MODULE ${MATRIX_SRCS})

