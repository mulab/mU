include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. DIFFERENTIAL_SRCS)
add_library(Differential ${PACKAGE_LIB_TYPE} ${DIFFERENTIAL_SRCS})

target_link_libraries(Differential mU gmp)

