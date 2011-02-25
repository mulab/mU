include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. MATRIX_SRCS)

add_library(Matrix ${PACKAGE_LIB_TYPE} ${MATRIX_SRCS})

target_link_libraries(Matrix mU Function gmp)

