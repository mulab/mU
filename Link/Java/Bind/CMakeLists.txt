include_directories(${JavaLink_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. JAVABIND_SRCS)
add_library(javabind SHARED ${JAVABIND_SRCS})
target_link_libraries(javabind javalink kernel parser)