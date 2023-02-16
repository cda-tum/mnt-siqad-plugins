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
set(QUICKSIM_SOURCES ${PROJECT_SOURCE_DIR}/src/main.cpp)

# Define the quicksim executable
add_executable(quicksim ${QUICKSIM_SOURCES})
# Link against the dependencies
target_link_libraries(quicksim dependencies)


# Build tests
option(QUICKSIM_TEST "Build QuickSim plugin tests" OFF)
if (QUICKSIM_TEST)
    enable_testing()
    message(STATUS "Building QuickSim tests")
    add_subdirectory(test)
endif ()


# SiQAD installation
if (DEFINED SIQAD_PLUGINS_ROOT)
    set(QUICKSIM_INSTALL_DIR "${SIQAD_PLUGINS_ROOT}/quicksim")
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
    set(QUICKSIM_INSTALL_DIR "debug")
else ()
    set(QUICKSIM_INSTALL_DIR "release")
endif ()


add_custom_command(
        TARGET quicksim POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/quicksim.physeng
        ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS quicksim RUNTIME DESTINATION ${QUICKSIM_INSTALL_DIR})
install(FILES quicksim.physeng DESTINATION ${QUICKSIM_INSTALL_DIR})

