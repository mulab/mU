include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. SYSTEM_SRCS)
add_library(system SHARED ${SYSTEM_SRCS})
target_link_libraries(system kernel parser)
