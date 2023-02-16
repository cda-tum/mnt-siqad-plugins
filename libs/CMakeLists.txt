add_library(dependencies INTERFACE)

# Include fiction
set(FICTION_CLI OFF CACHE BOOL "" FORCE)
set(FICTION_TEST OFF CACHE BOOL "" FORCE)
set(FICTION_EXPERIMENTS OFF CACHE BOOL "" FORCE)
set(FICTION_PROGRESS_BARS OFF CACHE BOOL "" FORCE)
add_subdirectory(fiction/)
target_link_libraries(dependencies INTERFACE libfiction)

# Include SiQADConn
target_include_directories(dependencies SYSTEM INTERFACE siqadconn/src/)

# Include Catch2
if (QUICKSIM_TEST)
    add_subdirectory(Catch2/)
endif ()

# Include Boost
if (WIN32)
    set(Boost_USE_STATIC_LIBS ON)
endif ()

# Require the Boost package
find_package(Boost REQUIRED)
if (Boost_FOUND)
    message(STATUS "Boost library has been found")
else ()
    message(FATAL_ERROR "Could not find Boost")
endif ()

# Link against Boost
target_include_directories(dependencies SYSTEM INTERFACE ${Boost_INCLUDE_DIRS})
