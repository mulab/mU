include_directories(${JavaLink_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. JAVAEMBED_SRCS)
add_library(javaembed SHARED ${JAVAEMBED_SRCS})
target_link_libraries(javaembed javalink kernel)
