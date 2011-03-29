include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. STREAM_SRCS)
add_library(Stream ${PACKAGE_LIB_TYPE} ${STREAM_SRCS})
                        
if(WIN32)
    set(SOCK_LIB wsock32 ws2_32)
else()
    set(SOCK_LIB "")
endif()

target_link_libraries(Stream mU gmp ${SOCK_LIB} ${Boost_LIBRARIES})

