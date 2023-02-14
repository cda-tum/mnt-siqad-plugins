cmake_minimum_required(VERSION 3.23)
project(quicksim)

# Set C++ standard; at least C++17 is required
#set(QUICKSIM_CXX_STANDARD "17" CACHE STRING "C++ standard")
#set(CMAKE_CXX_STANDARD ${QUICKSIM_CXX_STANDARD})
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(libs/fiction/)
add_subdirectory(libs/siqadconn/)

if (WIN32)
    set(Boost_USE_STATIC_LIBS ON)
endif()

find_package(Boost COMPONENTS random REQUIRED)
find_package(Threads REQUIRED)
include_directories(${Boost_INCLUDE_DIR})


set(QUICKSIM_SOURCES
        src/interface.h
        src/timer.h
        src/logger.h
        src/main.cpp
        )

add_executable(quicksim ${QUICKSIM_SOURCES})
# Add the submodules as subdirectories


target_link_libraries(quicksim libfiction)
include_directories(libs/siqadconn/)

add_dependencies(quicksim siqadconn)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs)

option(SIQAD_PLUGIN_TEST "Build plugin tests" OFF)
if (SIQAD_PLUGIN_TEST)
    enable_testing()
    message(STATUS "Building fiction tests")
    add_subdirectory(test)
endif ()

#target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

#if(DEFINED SIQAD_PLUGINS_ROOT)
#    set(QUICKSIM_INSTALL_DIR "${SIQAD_PLUGINS_ROOT}/quicksim")
#elseif(CMAKE_BUILD_TYPE MATCHES Debug)
#    set(QUICKSIM_INSTALL_DIR "debug")
#else()
#    set(QUICKSIM_INSTALL_DIR "release")
#ENDIF()


#add_custom_command(
#        TARGET quicksim POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy
#        ${CMAKE_CURRENT_SOURCE_DIR}/quicksim.physeng
#        ${CMAKE_CURRENT_BINARY_DIR})


#install(TARGETS simanneal RUNTIME DESTINATION ${CMAKE_BUILD_TYPE}/phys/simanneal)
#install(FILES simanneal.physeng DESTINATION ${CMAKE_BUILD_TYPE}/phys/simanneal)
#install(TARGETS quicksim RUNTIME DESTINATION ${QUICKSIM_INSTALL_DIR})
#install(FILES quicksim.physeng DESTINATION ${QUICKSIM_INSTALL_DIR})

