include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. FUNCTION_SRCS)
add_library(Function MODULE ${FUNCTION_SRCS})
target_link_libraries(Function mpfr)

