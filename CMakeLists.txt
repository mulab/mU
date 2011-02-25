cmake_minimum_required(VERSION 2.6)

project(mU CXX)

set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package( Boost 1.44.0 COMPONENTS system iostreams )

if(NOT Boost_FOUND)
    message(SEND_ERROR "boost library not found")
endif()
   
set(mU_HEADER_DIR ${mU_SOURCE_DIR}/Include ${Boost_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "-Wno-deprecated")

set(EXECUTABLE_OUTPUT_PATH ..)
set(LIBRARY_OUTPUT_PATH ..)

if(WIN32)
    set(PACKAGE_LIB_TYPE SHARED)
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
    set(CMAKE_SHARED_LIBRARY_SUFFIX ".dll")
elseif(UNIX)
    set(PACKAGE_LIB_TYPE MODULE)
    set(CMAKE_SHARED_MODULE_PREFIX "")
    set(CMAKE_SHARED_MODULE_SUFFIX ".dll")
else()
    message(FATAL_ERROR "building on an unsupported operating system")
endif() 

add_subdirectory(Kernel)
add_subdirectory(Package)
