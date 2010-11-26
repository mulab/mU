include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. POLYNOMIAL_SRCS)
add_library(polynomial SHARED ${POLYNOMIAL_SRCS})
target_link_libraries(polynomial kernel number) 
