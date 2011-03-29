include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. FUNCTION_SRCS)

# FIXME: which library type should be used here? use SHARED for now
add_library(Function SHARED ${FUNCTION_SRCS})

target_link_libraries(Function mU gmp mpfr)

