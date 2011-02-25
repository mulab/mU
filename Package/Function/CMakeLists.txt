include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. FUNCTION_SRCS)

add_library(Function ${PACKAGE_LIB_TYPE} ${FUNCTION_SRCS})
set_target_properties(Function PROPERTIES 
                        COMPILE_DEFINITIONS "FUNCTION_EXPORTS")

target_link_libraries(Function mU gmp mpfr)

