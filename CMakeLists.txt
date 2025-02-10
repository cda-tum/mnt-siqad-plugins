cmake_minimum_required(VERSION 3.21...3.29)

project(mnt-siqad-plugins
        VERSION 0.1
        LANGUAGES CXX C)

# Only set the CMAKE_CXX_STANDARD if it is not set by someone else
if (NOT DEFINED CMAKE_CXX_STANDARD)
    # Set C++ standard; at least C++17 is required
    set(CMAKE_CXX_STANDARD 17)
endif ()

# Load standard project settings
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/StandardProjectSettings.cmake)

# Add the include directory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add external libraries folder
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs)

# Add the plugins folder
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/plugins)

# Build tests
option(MNT_SIQAD_PLUGINS_TEST "Build SiQAD plugin tests" OFF)
if (MNT_SIQAD_PLUGINS_TEST)
    enable_testing()
    message(STATUS "Building SiQAD plugin tests")
    add_subdirectory(test)
endif ()
