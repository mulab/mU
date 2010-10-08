include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. KERNEL_SRCS)
add_library(kernel SHARED ${KERNEL_SRCS})
target_link_libraries(kernel dl gmp mpfr) 
