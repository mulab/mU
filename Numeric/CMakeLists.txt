include_directories(${mU_HEADER_DIR})
AUX_SOURCE_DIRECTORY(. NUMERIC_SRCS)
add_library(numeric SHARED ${NUMERIC_SRCS})
target_link_libraries(numeric mpfr kernel polynomial)
