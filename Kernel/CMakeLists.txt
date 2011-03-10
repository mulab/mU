include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. KERNEL_SRCS)
add_library(kernel SHARED ${KERNEL_SRCS})

if(WIN32)
    target_link_libraries(kernel gmp)
elseif(UNIX)
    target_link_libraries(kernel gmp dl)
else()
    message(FATAL_ERROR "building on an unsupported operating system")
endif()

