include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. PARSER_SRCS)
add_library(parser SHARED ${PARSER_SRCS})
target_link_libraries(parser kernel)
