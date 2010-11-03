include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. FUNCTION_SRCS)
add_library(function SHARED ${FUNCTION_SRCS})
target_link_libraries(function kernel mpfr) 
