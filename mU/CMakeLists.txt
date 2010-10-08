include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. MU_SRCS)
add_executable(mu ${MU_SRCS})
target_link_libraries(mu kernel parser)
