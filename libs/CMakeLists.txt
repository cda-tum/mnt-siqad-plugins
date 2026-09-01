add_library(mnt-siqad-plugins-dependencies INTERFACE)

# Fetch fiction, siqadconn, and Catch2
include(${CMAKE_SOURCE_DIR}/cmake/Dependencies.cmake)

# Include fiction
target_link_libraries(mnt-siqad-plugins-dependencies INTERFACE libfiction)

# Include SiQADConn
target_include_directories(mnt-siqad-plugins-dependencies SYSTEM
                           INTERFACE ${siqadconn_SOURCE_DIR}/src)

# Require the Boost package
find_package(Boost 1.70.0 REQUIRED)
if(Boost_FOUND)
  message(STATUS "Boost libraries have been found: ${Boost_VERSION}")

  # Link against Boost
  target_link_libraries(mnt-siqad-plugins-dependencies INTERFACE Boost::boost)
else()
  message(FATAL_ERROR "Could not find Boost")
endif()
