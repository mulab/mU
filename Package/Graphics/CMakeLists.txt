include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. GRAPHICS_SRCS)
add_library(Graphics MODULE ${GRAPHICS_SRCS})

