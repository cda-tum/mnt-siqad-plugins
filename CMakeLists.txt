cmake_minimum_required(VERSION 3.13)

project(quicksim-siqad-plugin
        VERSION 1.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Load standard project settings
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/StandardProjectSettings.cmake)

# Include libraries
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs)

# QuickSim source files
set(QUICKEXACT_SOURCES ${PROJECT_SOURCE_DIR}/src/main.cpp)

# Define the quicksim executable
add_executable(quickexact ${QUICKEXACT_SOURCES})
# Link against the dependencies
target_link_libraries(quickexact dependencies)


# Build tests
option(QUICKEXACT_TEST "Build QuickSim plugin tests" OFF)
if (QUICKEXACT_TEST)
    enable_testing()
    message(STATUS "Building QuickEXACT tests")
    add_subdirectory(test)
endif ()


# SiQAD installation
if (DEFINED SIQAD_PLUGINS_ROOT)
    set(QUICKEXACT_INSTALL_DIR "${SIQAD_PLUGINS_ROOT}/quickexact")
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
    set(QUICKEXACT_INSTALL_DIR "debug")
else ()
    set(QUICKEXACT_INSTALL_DIR "release")
endif ()


add_custom_command(
        TARGET quickexact POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/quickexact.physeng
        ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS quickexact RUNTIME DESTINATION ${QUICKEXACT_INSTALL_DIR})
install(FILES quickexact.physeng DESTINATION ${QUICKEXACT_INSTALL_DIR})

