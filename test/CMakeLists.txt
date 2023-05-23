file(GLOB_RECURSE FILENAMES *.cpp)

add_library(cda-tum-siqad-plugins-testlib INTERFACE)
target_compile_definitions(cda-tum-siqad-plugins-testlib INTERFACE CATCH_CONFIG_NO_POSIX_SIGNALS) # make catch2 ignore SIGTERMs sent to applications when timeouts are reached
target_compile_definitions(cda-tum-siqad-plugins-testlib INTERFACE "TEST_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/\"")

foreach (FILE IN LISTS FILENAMES)
    get_filename_component(NAME ${FILE} NAME_WE)
    set(TEST_NAME test_${NAME})
    add_executable(${TEST_NAME} ${FILE})
    target_link_libraries(${TEST_NAME} PRIVATE Catch2::Catch2WithMain cda-tum-siqad-plugins-include cda-tum-siqad-plugins-dependencies cda-tum-siqad-plugins-testlib)
    add_test(NAME ${NAME} COMMAND ${TEST_NAME}) # group tests by file
endforeach ()
