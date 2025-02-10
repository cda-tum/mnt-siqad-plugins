file(GLOB_RECURSE FILENAMES *.cpp)

add_library(mnt-siqad-plugins-testlib INTERFACE)
target_compile_definitions(mnt-siqad-plugins-testlib INTERFACE CATCH_CONFIG_NO_POSIX_SIGNALS) # make catch2 ignore SIGTERMs sent to applications when timeouts are reached
target_compile_definitions(mnt-siqad-plugins-testlib INTERFACE "TEST_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/\"")

foreach (FILE IN LISTS FILENAMES)
    get_filename_component(NAME ${FILE} NAME_WE)
    set(TEST_NAME test_${NAME})
    add_executable(${TEST_NAME} ${FILE})
    target_link_libraries(${TEST_NAME} PRIVATE Catch2::Catch2WithMain mnt-siqad-plugins-include mnt-siqad-plugins-dependencies mnt-siqad-plugins-testlib)
    add_test(NAME ${NAME} COMMAND ${TEST_NAME}) # group tests by file

    if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        add_custom_command(TARGET ${TEST_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:Catch2>" $<TARGET_FILE_DIR:${TEST_NAME}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:Catch2WithMain>" $<TARGET_FILE_DIR:${TEST_NAME}>
        )
    endif ()
endforeach ()
