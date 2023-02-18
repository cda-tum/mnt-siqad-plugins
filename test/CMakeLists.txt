include_directories(.)
file(GLOB_RECURSE FILENAMES *.cpp)

add_library(testlib INTERFACE)
target_compile_definitions(testlib INTERFACE CATCH_CONFIG_NO_POSIX_SIGNALS) # make catch2 ignore SIGTERMs sent to applications when timeouts are reached
target_compile_definitions(testlib INTERFACE "TEST_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/\"")
target_include_directories(testlib INTERFACE ${PROJECT_SOURCE_DIR}/src/)

foreach (FILE IN LISTS FILENAMES)
    get_filename_component(NAME ${FILE} NAME_WE)
    set(TEST_NAME test_${NAME})
    add_executable(${TEST_NAME} ${FILE})
    target_link_libraries(${TEST_NAME} PRIVATE Catch2::Catch2WithMain dependencies testlib)
    add_test(NAME ${NAME} COMMAND ${TEST_NAME}) # group tests by file
endforeach ()
