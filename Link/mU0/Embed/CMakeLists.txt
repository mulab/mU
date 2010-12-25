include_directories(${mU0Link_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. MU0EMBED_SRCS)
add_library(mu0embed SHARED ${MU0EMBED_SRCS})
target_link_libraries(mu0embed mu0link kernel)
