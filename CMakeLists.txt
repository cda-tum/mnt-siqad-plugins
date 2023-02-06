cmake_minimum_required(VERSION 3.20)

# Set C++ standard; at least C++17 is required
set(FICTION_CXX_STANDARD "17" CACHE STRING "C++ standard")
set(CMAKE_CXX_STANDARD ${FICTION_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(quicksim-siqad-plugin)

add_executable(quicksim-siqad-plugin src/main.cpp)

set(QUICKSIM_SOURCES
        src/main.ccp
        src/interface.h
        src/timer.h
        src/logger.h
        )

include_directories(libs/siqadconn/)

# Add the submodules as subdirectories
add_subdirectory(libs/fiction/)
add_subdirectory(libs/siqadconn/)

target_link_libraries(quicksim-siqad-plugin libfiction)

set(Boost_INCLUDE_DIR c:/mingw/include/)
set(Boost_LIBRARY_DIR c:/mingw/lib/)
find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_dependencies(quicksim-siqad-plugin siqadconn)

