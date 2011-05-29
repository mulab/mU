include_directories(${mU_HEADER_DIR})

set(KERNEL_SRCS 
        box.cpp
        coefficient.cpp
        compare.cpp
        do.cpp
        dot.cpp
        eval.cpp
        evalf.cpp
        expand.cpp
        gen.cpp
        initialize.cpp
        Interface.cpp
        number.cpp
        package.cpp
        parser.cpp
        path.cpp
        pattern.cpp
        plus.cpp
        power.cpp
        pretty.cpp
        print.cpp
        scan.cpp
        string.cpp
        times.cpp
)
add_library(Kernel ${PACKAGE_LIB_TYPE} ${KERNEL_SRCS})

set_target_properties(Kernel PROPERTIES
                            COMPILE_DEFINITIONS "KERNEL_EXPORTS"
                            OUTPUT_NAME "kernel")

if(WIN32)
    target_link_libraries(Kernel gmp ${Boost_LIBRARIES})
elseif(UNIX)
    target_link_libraries(Kernel pthread gmp dl ${Boost_LIBRARIES})
else()
    message(FATAL_ERROR "building on an unsupported operating system")
endif()

set(MU_SRCS main.cpp)
add_executable(mU ${MU_SRCS})
if(WIN32)
    target_link_libraries(mU Kernel gmp)
elseif(UNIX)
    target_link_libraries(mU Kernel gmp dl)
else()
    message(FATAL_ERROR "building on an unsupported operating system")
endif()
