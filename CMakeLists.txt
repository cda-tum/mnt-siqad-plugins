cmake_minimum_required(VERSION 3.13)

project(cda-tum-siqad-plugins
        VERSION 1.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Load standard project settings
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/StandardProjectSettings.cmake)

# Add the include directory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add external libraries folder
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs)

# Add the plugins folder
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/plugins)

# Build tests
option(CDA_TUM_SIQAD_PLUGINS_TEST "Build SiQAD plugin tests" OFF)
if (CDA_TUM_SIQAD_PLUGINS_TEST)
    enable_testing()
    message(STATUS "Building SiQAD plugin tests")
    add_subdirectory(test)
endif ()
