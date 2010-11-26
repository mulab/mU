include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. COMBINATORICS_SRCS)
add_library(combinatorics SHARED ${COMBINATORICS_SRCS})
target_link_libraries(combinatorics kernel) 
