cmake_minimum_required(VERSION 3.23)

project(quicksim)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(libs/fiction/)


if (WIN32)
    set(Boost_USE_STATIC_LIBS ON)
endif()

find_package(Boost COMPONENTS random REQUIRED)

include_directories(
        .
        src
        ${Boost_INCLUDE_DIR}
)

set(QUICKSIM_SOURCES
        src/main.cpp
        )

add_executable(quicksim ${QUICKSIM_SOURCES})

# Add the submodules as subdirectories
target_link_libraries(quicksim libfiction)
include_directories(libs/siqadconn/)


option(SIQAD_PLUGIN_TEST "Build plugin tests" OFF)
if (SIQAD_PLUGIN_TEST)
    enable_testing()
    message(STATUS "Building fiction tests")
    add_subdirectory(test)
endif ()


if(DEFINED SIQAD_PLUGINS_ROOT)
    set(QUICKSIM_INSTALL_DIR "${SIQAD_PLUGINS_ROOT}/quicksim")
elseif(CMAKE_BUILD_TYPE MATCHES Debug)
    set(QUICKSIM_INSTALL_DIR "debug")
else()
    set(QUICKSIM_INSTALL_DIR "release")
ENDIF()
