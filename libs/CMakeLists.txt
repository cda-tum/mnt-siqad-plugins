add_library(mnt-siqad-plugins-dependencies INTERFACE)

# Fetch fiction, siqadconn, and Catch2.
include(${CMAKE_SOURCE_DIR}/cmake/Dependencies.cmake)

find_package(Boost 1.70.0 REQUIRED)
message(STATUS "Boost libraries have been found: ${Boost_VERSION}")

# Compile the compatible SiQADConn source once for all plugins and tests.
add_library(mnt-siqad-plugins-siqadconn STATIC
            "${siqadconn_BINARY_DIR}/siqadconn.cc")
target_include_directories(mnt-siqad-plugins-siqadconn SYSTEM
                           PUBLIC "${siqadconn_BINARY_DIR}")
target_link_libraries(mnt-siqad-plugins-siqadconn PUBLIC Boost::boost)
target_link_libraries(mnt-siqad-plugins-dependencies
                      INTERFACE libfiction mnt-siqad-plugins-siqadconn)
