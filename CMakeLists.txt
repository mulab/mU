cmake_minimum_required(VERSION 2.6)

project(mU CXX)

if ("${CMAKE_GENERATOR}" MATCHES "MinGW")
    set(MinGW TRUE)
else()
    set(MinGW FALSE)
endif()

if (MinGW)
    set(POSSIBLE_LIBRARY_PATH 
        "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MinGW;InstallLocation]/lib" 
        "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{AC2C1BDB-1E91-4F94-B99C-E716FE2E9C75}_is1;InstallLocation]/lib" 
        c:/MinGW/lib /MinGW/lib)
else()
    set(POSSIBLE_LIBRARY_PATH "")
endif()

find_library(GMP gmp PATHS ${POSSIBLE_LIBRARY_PATH})
find_library(MPFR mpfr PATHS ${POSSIBLE_LIBRARY_PATH})

if(WIN32)
    set(Boost_USE_STATIC_LIBS    ON)
elseif(UNIX)
    set(Boost_USE_STATIC_LIBS   OFF)
else()
    message(FATAL_ERROR "building on an unsupported operating system")
endif()
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package( Boost 1.42.0 COMPONENTS system )

if(NOT Boost_FOUND)
    message(SEND_ERROR "boost library not found")
    # TODO: reference to "developer getting started" page
endif()
   
set(mU_HEADER_DIR ${mU_SOURCE_DIR}/Include ${Boost_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "-Wno-deprecated")

set(EXECUTABLE_OUTPUT_PATH ..)
set(LIBRARY_OUTPUT_PATH ..)
    
# FIXME: can we simply set the library type to SHARED?
# issue 1: what's the difference between SHARED and MODULE on UNIX gcc
#          and MinGW gcc?
# issue 2: MODULEs can't be linked to other MODULEs, but we need to link
#          Function.dll into Matrix.dll
if(WIN32)
    set(PACKAGE_LIB_TYPE SHARED)
elseif(UNIX)
    set(PACKAGE_LIB_TYPE SHARED) 
else()
    message(FATAL_ERROR "building on an unsupported operating system")
endif() 

add_subdirectory(Kernel)
add_subdirectory(Package)
