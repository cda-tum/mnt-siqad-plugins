# Set a default build type if none was specified
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE
            Release
            CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui, ccmake
    set_property(
            CACHE CMAKE_BUILD_TYPE
            PROPERTY STRINGS
            "Debug"
            "Release"
            "MinSizeRel"
            "RelWithDebInfo")
endif ()

# Generate compile_commands.json to make it easier to work with clang based tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(CDA_TUM_SIQAD_PLUGINS_ENABLE_IPO "Enable Interprocedural Optimization, aka Link Time Optimization (LTO)" OFF)

if (CDA_TUM_SIQAD_PLUGINS_ENABLE_IPO)
    include(CheckIPOSupported)
    check_ipo_supported(
            RESULT
            ipo_supported
            OUTPUT
            ipo_output)
    # enable inter-procedural optimization if it is supported (Clang's ThinLTO does not work with Ubuntu 20.04's default linker at the moment)
    if ((ipo_supported AND NOT ((${CMAKE_SYSTEM_NAME} MATCHES "Linux") AND (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang"))))
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    else ()
        message(SEND_ERROR "IPO is not supported: ${ipo_output}")
    endif ()
endif ()
