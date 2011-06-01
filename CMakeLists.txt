cmake_minimum_required(VERSION 2.6)

project(mU CXX)

if ("${CMAKE_CXX_PLATFORM_ID}" MATCHES "MinGW")
    set(MinGW TRUE)
else()
    set(MinGW FALSE)
endif()

if (MinGW)
    set(POSSIBLE_LIBRARY_DIRS 
        "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MinGW;InstallLocation]/lib" 
        "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{AC2C1BDB-1E91-4F94-B99C-E716FE2E9C75}_is1;InstallLocation]/lib" 
        c:/MinGW/lib /MinGW/lib)
else()
    set(POSSIBLE_LIBRARY_DIRS "")
endif()

if(UNIX OR MinGW)
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} "-E" "-P" "-v" "-dD" 
                                "${CMAKE_ROOT}/Modules/DummyCXXFile.cxx"
                OUTPUT_VARIABLE DISCOVERED_CONFIGURATION
                ERROR_VARIABLE DISCOVERED_CONFIGURATION)
    string(REGEX REPLACE ".*> search starts here:(.*)End of search list.*"
            "\\1" DISCOVERED_INCLUDE_DIRS ${DISCOVERED_CONFIGURATION})
    string(STRIP ${DISCOVERED_INCLUDE_DIRS} DISCOVERED_INCLUDE_DIRS)
#    string(REGEX REPLACE ".*End of search list.(.*)COMPILER_PATH.*" "\\1"
#                DISCOVERED_DEFINES ${DISCOVERED_CONFIGURATION})
#    string(REGEX REPLACE "^#define [^ ]+\\([^$]*$" ""
#                DISCOVERED_DEFINES ${DISCOVERED_DEFINES})
#    string(REGEX REPLACE "^#define ([^ ]+) (.+)$" "^\\1=\\2$"
#                DISCOVERED_DEFINES ${DISCOVERED_DEFINES})
#    string(REGEX REPLACE "^#define ([^ ]+)$" "^\\1$"
#                DISCOVERED_DEFINESS ${DISCOVERED_DEFINES})
endif()

find_library(GMP gmp PATHS ${POSSIBLE_LIBRARY_DIRS})
find_library(MPFR mpfr PATHS ${POSSIBLE_LIBRARY_DIRS})

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

set(mU_HEADER_DIR ${DISCOVERED_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${mU_SOURCE_DIR}/Include)
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
