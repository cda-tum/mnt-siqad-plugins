cmake_minimum_required(VERSION 3.20)

project(quicksim-siqad-plugin)

add_executable(quicksim-siqad-plugin src/main.cpp)

set(QUICKSIM_SOURCES
        src/main.ccp
        src/global.cc
        src/interface.h
        )

# Add the submodules as subdirectories
add_subdirectory(libs/fiction/)
add_subdirectory(libs/siqadconn)

target_link_libraries(quicksim-siqad-plugin libfiction)

set(Boost_INCLUDE_DIR c:/mingw/include/)
set(Boost_LIBRARY_DIR c:/mingw/lib/)
find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_dependencies(quicksim-siqad-plugin siqadconn)

#target_link_libraries(quicksim-siqad-plugin siqadconn)
#target_link_libraries(quicksim-siqad-plugin siqadconn ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
