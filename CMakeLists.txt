cmake_minimum_required(VERSION 3.20)
project(quicksim-siqad-plugin)

# Set C++ standard; at least C++17 is required
set(FICTION_CXX_STANDARD "17" CACHE STRING "C++ standard")
set(CMAKE_CXX_STANDARD ${FICTION_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)



if (WIN32)
    set(Boost_USE_STATIC_LIBS ON)
endif()
find_package(Boost COMPONENTS random REQUIRED)
find_package(Threads REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(quicksim-siqad-plugin src/main.cpp)


set(QUICKSIM_SOURCES
        src/interface.h
        src/timer.h
        src/logger.h
        src/main.cpp
        )

# Add the submodules as subdirectories
add_subdirectory(libs/fiction/)
add_subdirectory(libs/siqadconn/)

target_link_libraries(quicksim-siqad-plugin libfiction)
include_directories(libs/siqadconn/)

add_dependencies(quicksim-siqad-plugin siqadconn)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs)

option(SIQAD_PLUGIN_TEST "Build plugin tests" OFF)
if (SIQAD_PLUGIN_TEST)
    enable_testing()
    message(STATUS "Building fiction tests")
    add_subdirectory(test)
endif ()

