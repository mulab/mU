include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. KERNEL_SRCS)
add_executable(mU ${KERNEL_SRCS})
target_link_libraries(mU pthread gmp dl)
