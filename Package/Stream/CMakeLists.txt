include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. STREAM_SRCS)
add_library(Stream MODULE ${STREAM_SRCS})
target_link_libraries(Stream boost_system)

