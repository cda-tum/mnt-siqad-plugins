cmake_minimum_required(VERSION 3.21...4.2)

# Only set the CMAKE_CXX_STANDARD if it is not set by someone else
if(NOT DEFINED CMAKE_CXX_STANDARD)
  # Set C++ standard; at least C++17 is required.
  #
  # fiction itself has since raised its own minimum to C++20, and this value
  # must not be lower than fiction's own standard: fiction's CMakeLists.txt only
  # sets CMAKE_CXX_STANDARD if it is not already defined, so staying at 17 will
  # silently downgrade fiction's own build the next time its submodule pin is
  # bumped. Bumping this to 20 was tried and reverted: it broke the macOS build
  # with `invalid application of 'sizeof' to an incomplete type 'phys::Layer'`
  # in libs/siqadconn/src/siqadconn.h, a pre-existing forward-declaration issue
  # in the (third-party, unpinned by us beyond its own submodule commit)
  # siqadconn dependency that C++20's stricter libc++ allocator only newly
  # exposes. Revisit once siqadconn is fixed upstream or the fiction submodule
  # pin actually advances past C++17.
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
