include_directories(${EclLink_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. ECLEMBED_SRCS)
add_library(eclembed SHARED ${ECLEMBED_SRCS})
target_link_libraries(eclembed ecllink kernel) 
