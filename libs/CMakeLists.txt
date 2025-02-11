add_library(mnt-siqad-plugins-dependencies INTERFACE)

# Include fiction
set(FICTION_CLI OFF CACHE BOOL "" FORCE)
set(FICTION_PROGRESS_BARS OFF CACHE BOOL "" FORCE)
set(FICTION_ALGLIB ON CACHE BOOL "" FORCE)  # Enable ALGLIB by default to support ClusterComplete
set(MOCKTURTLE_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/fiction)
target_link_libraries(mnt-siqad-plugins-dependencies INTERFACE libfiction)

# Include SiQADConn
target_include_directories(mnt-siqad-plugins-dependencies SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/siqadconn/src)

# Include Catch2
if (MNT_SIQAD_PLUGINS_TEST)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Catch2)
endif ()

# Require the Boost package
find_package(Boost 1.70.0 REQUIRED)
if (Boost_FOUND)
    message(STATUS "Boost libraries have been found: ${Boost_VERSION}")

    # Link against Boost
    target_link_libraries(mnt-siqad-plugins-dependencies INTERFACE Boost::boost)
else ()
    message(FATAL_ERROR "Could not find Boost")
endif ()
