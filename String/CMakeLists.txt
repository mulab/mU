include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. STRING_SRCS)
add_library(string SHARED ${STRING_SRCS})
target_link_libraries(string kernel) 
