cmake_minimum_required(VERSION 2.6)

project(LinkEcl CXX)

AUX_SOURCE_DIRECTORY(. LINKECL_SRCS)
add_library(linkecl SHARED ${LINKECL_SRCS})
target_link_libraries(linkecl kernel ecl)