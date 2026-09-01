# Enable a compiler cache (ccache/sccache) if one is available
function(mnt_siqad_plugins_enable_cache)
  set(CACHE_OPTION
      "ccache"
      CACHE STRING "Compiler cache to be used")
  set(CACHE_OPTION_VALUES "ccache" "sccache")
  set_property(CACHE CACHE_OPTION PROPERTY STRINGS ${CACHE_OPTION_VALUES})
  list(FIND CACHE_OPTION_VALUES ${CACHE_OPTION} CACHE_OPTION_INDEX)

  if(${CACHE_OPTION_INDEX} EQUAL -1)
    message(
      STATUS
        "Using custom compiler cache system: '${CACHE_OPTION}', explicitly supported entries are ${CACHE_OPTION_VALUES}"
    )
  endif()

  find_program(CACHE_BINARY NAMES ${CACHE_OPTION})
  if(NOT CACHE_BINARY)
    list(REMOVE_ITEM CACHE_OPTION_VALUES ${CACHE_OPTION})
    find_program(CACHE_BINARY NAMES ${CACHE_OPTION_VALUES})
    if(CACHE_BINARY)
      message(STATUS "${CACHE_OPTION} was not found, falling back to "
                     "${CACHE_BINARY}")
    endif()
  endif()

  if(CACHE_BINARY)
    # The Visual Studio generator ignores compiler launchers.
    if(CMAKE_GENERATOR MATCHES "Visual Studio")
      message(
        WARNING
          "${CACHE_BINARY} was found, but the '${CMAKE_GENERATOR}' generator ignores "
          "compiler launchers, so nothing will be cached. Configure with -G Ninja "
          "to make the cache effective.")
    endif()
    message(STATUS "${CACHE_BINARY} found and enabled")

    set(CMAKE_CXX_COMPILER_LAUNCHER
        ${CACHE_BINARY}
        CACHE STRING "CXX compiler cache used")
    set(CMAKE_C_COMPILER_LAUNCHER
        ${CACHE_BINARY}
        CACHE STRING "C compiler cache used")
  else()
    message(
      WARNING "${CACHE_OPTION} is enabled but was not found. Not using it")
  endif()
endfunction()
