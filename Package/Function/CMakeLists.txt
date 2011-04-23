include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. FUNCTION_SRCS)

# FIXME: which library type should be used here? use SHARED for now
add_library(Function SHARED ${FUNCTION_SRCS})
set_target_properties(Function PROPERTIES 
                        COMPILE_DEFINITIONS "FUNCTION_EXPORTS")

target_link_libraries(Function mU gmp mpfr)

