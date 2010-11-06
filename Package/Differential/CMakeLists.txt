include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. DIFFERENTIAL_SRCS)
add_library(Differential MODULE ${DIFFERENTIAL_SRCS})

