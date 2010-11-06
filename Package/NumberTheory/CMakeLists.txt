include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. NUMBERTHEORY_SRCS)
add_library(NumberTheory MODULE ${NUMBERTHEORY_SRCS})

