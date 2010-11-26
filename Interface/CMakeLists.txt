include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. INTERFACE_SRCS)
add_library(interface SHARED ${INTERFACE_SRCS})
target_link_libraries(interface kernel dl)
