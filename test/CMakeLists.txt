file(GLOB_RECURSE FILENAMES *.cpp)

add_library(mnt-siqad-plugins-testlib INTERFACE)
target_compile_definitions(
  mnt-siqad-plugins-testlib INTERFACE CATCH_CONFIG_NO_POSIX_SIGNALS
)# make catch2 ignore SIGTERMs sent to applications when timeouts are reached
target_compile_definitions(
  mnt-siqad-plugins-testlib
  INTERFACE "TEST_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/\"")

foreach(FILE IN LISTS FILENAMES)
  get_filename_component(NAME ${FILE} NAME_WE)
  set(TEST_NAME test_${NAME})
  add_executable(${TEST_NAME} ${FILE})
  target_link_libraries(
    ${TEST_NAME}
    PRIVATE Catch2::Catch2WithMain mnt-siqad-plugins-include
            mnt-siqad-plugins-dependencies mnt-siqad-plugins-testlib)
  add_test(NAME ${NAME} COMMAND ${TEST_NAME}) # group tests by file

  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_custom_command(
      TARGET ${TEST_NAME}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:Catch2>"
              $<TARGET_FILE_DIR:${TEST_NAME}>
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              "$<TARGET_FILE:Catch2WithMain>" $<TARGET_FILE_DIR:${TEST_NAME}>)
  endif()
endforeach()

# A failed heuristic search must reach the caller as a nonzero plugin exit.
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/quicksim_problem_0.xml"
     NO_RESULT_PROBLEM)
string(REPLACE "<iteration_steps>70</iteration_steps>"
               "<iteration_steps>0</iteration_steps>" NO_RESULT_PROBLEM
               "${NO_RESULT_PROBLEM}")
file(CONFIGURE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/quicksim_no_results.xml"
     CONTENT "${NO_RESULT_PROBLEM}" @ONLY)
add_test(NAME quicksim_no_results
         COMMAND quicksim "${CMAKE_CURRENT_BINARY_DIR}/quicksim_no_results.xml"
                 "${CMAKE_CURRENT_BINARY_DIR}/quicksim_no_results_out.xml")
set_tests_properties(quicksim_no_results PROPERTIES WILL_FAIL TRUE)
