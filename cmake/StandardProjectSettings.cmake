# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE
      Release
      CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui, ccmake
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
                                               "MinSizeRel" "RelWithDebInfo")
endif()

# Generate compile_commands.json to make it easier to work with clang based
# tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enhance error reporting and compiler messages
if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  if(WIN32)
    # On Windows cuda nvcc uses cl and not clang
    add_compile_options($<$<COMPILE_LANGUAGE:C>:-fcolor-diagnostics>
                        $<$<COMPILE_LANGUAGE:CXX>:-fcolor-diagnostics>)
  else()
    add_compile_options(-fcolor-diagnostics)
  endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(WIN32)
    # On Windows cuda nvcc uses cl and not gcc
    add_compile_options($<$<COMPILE_LANGUAGE:C>:-fdiagnostics-color=always>
                        $<$<COMPILE_LANGUAGE:CXX>:-fdiagnostics-color=always>)
  else()
    add_compile_options(-fdiagnostics-color=always)
  endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND MSVC_VERSION GREATER 1900)
  add_compile_options(/diagnostics:column)
else()
  message(
    STATUS
      "No colored compiler diagnostic set for '${CMAKE_CXX_COMPILER_ID}' compiler."
  )
endif()

# Use bigobj for MSVC due to many inline and template functions
if(MSVC)
  add_compile_options(/bigobj)
endif()

option(MNT_SIQAD_PLUGINS_ENABLE_CACHE
       "Enable a compiler cache (ccache/sccache) if available" ON)
if(MNT_SIQAD_PLUGINS_ENABLE_CACHE)
  include(${CMAKE_CURRENT_LIST_DIR}/Cache.cmake)
  mnt_siqad_plugins_enable_cache()
endif()

option(MNT_SIQAD_PLUGINS_ENABLE_IPO
       "Enable Interprocedural Optimization, aka Link Time Optimization (LTO)"
       OFF)

if(MNT_SIQAD_PLUGINS_ENABLE_IPO)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT ipo_supported OUTPUT ipo_output)
  # enable inter-procedural optimization if it is supported (Clang's ThinLTO
  # does not work with Ubuntu 20.04's default linker at the moment)
  if((ipo_supported AND NOT ((${CMAKE_SYSTEM_NAME} MATCHES "Linux")
                             AND (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang"))))
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  else()
    message(SEND_ERROR "IPO is not supported: ${ipo_output}")
  endif()
endif()
