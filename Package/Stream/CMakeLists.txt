include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. STREAM_SRCS)
add_library(Stream ${PACKAGE_LIB_TYPE} ${STREAM_SRCS})
                        
if(WIN32)
    set(SOCK_LIB wsock32 ws2_32)
else()
    set(SOCK_LIB "")
endif()

# FIXME: use Boost_LIBRARIES instead boost_system-mgw45-mt-1_45
if(WIN32)
    target_link_libraries(Stream mU gmp ${SOCK_LIB} boost_system-mgw45-mt-1_45) 
elseif(UNIX)
    target_link_libraries(Stream mU gmp ${SOCK_LIB} boost_system)
else()
    message(FATAL_ERROR "building on an unsupported operating system")
endif()

