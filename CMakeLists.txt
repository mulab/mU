cmake_minimum_required(VERSION 2.6)

project(mU CXX)

# TODO: also check for the availability of gmp and mpfr libraries

set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package( Boost 1.42.0 COMPONENTS system iostreams )

if(NOT Boost_FOUND)
    message(SEND_ERROR "boost library not found")
endif()
   
set(mU_HEADER_DIR ${mU_SOURCE_DIR}/Include ${Boost_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "-Wno-deprecated")

set(EXECUTABLE_OUTPUT_PATH ..)
set(LIBRARY_OUTPUT_PATH ..)

set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".dll")
set(CMAKE_SHARED_MODULE_PREFIX "")
set(CMAKE_SHARED_MODULE_SUFFIX ".dll")
    
# FIXME: can we simply set the library type to SHARED?
# issue 1: what's the difference between SHARED and MODULE on UNIX gcc
#          and MinGW gcc?
# issue 2: MODULEs can't be linked to other MODULEs, but we need to link
#          Function.dll into Matrix.dll
if(WIN32)
    set(PACKAGE_LIB_TYPE SHARED)
elseif(UNIX)
    set(PACKAGE_LIB_TYPE MODULE) 
else()
    message(FATAL_ERROR "building on an unsupported operating system")
endif() 

add_subdirectory(Kernel)
add_subdirectory(Package)
