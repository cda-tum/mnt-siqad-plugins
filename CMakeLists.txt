cmake_minimum_required(VERSION 3.21...4.2)

# Only set the CMAKE_CXX_STANDARD if it is not set by someone else
if(NOT DEFINED CMAKE_CXX_STANDARD)
  # Set C++ standard; at least C++17 is required
  set(CMAKE_CXX_STANDARD 17)
endif()

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++17 and -std=gnu++17 for example
set(CMAKE_CXX_EXTENSIONS OFF)

# With the Ninja generator, CMake >= 3.28 enables C++20 module dependency
# scanning by default, which requires a `clang-scan-deps` binary matching the
# compiler version. This project uses no C++20 modules, and CI images don't
# always ship a matching `clang-scan-deps`, so disable it.
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

project(
  mnt-siqad-plugins
  VERSION 0.2.0
  LANGUAGES CXX C)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PreventInSourceBuilds.cmake)

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
if(MNT_SIQAD_PLUGINS_TEST)
  enable_testing()
  message(STATUS "Building SiQAD plugin tests")
  add_subdirectory(test)
endif()
