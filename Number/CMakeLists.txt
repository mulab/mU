include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. NUMBER_SRCS)
add_library(number SHARED ${NUMBER_SRCS})
target_link_libraries(number kernel) 
