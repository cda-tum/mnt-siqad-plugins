file(GLOB_RECURSE FILENAMES *.cpp)

add_library(mnt-siqad-plugins-testlib INTERFACE)
target_compile_definitions(
  mnt-siqad-plugins-testlib INTERFACE CATCH_CONFIG_NO_POSIX_SIGNALS
)# make catch2 ignore SIGTERMs sent to applications when timeouts are reached
target_compile_definitions(
  mnt-siqad-plugins-testlib
  INTERFACE "TEST_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/\""
            "TEST_OUTPUT_PATH=\"${CMAKE_CURRENT_BINARY_DIR}/\"")

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

# Check both the exit status and diagnostic from the actual plugin.
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/quicksim_problem_0.xml" QUICKSIM_PROBLEM)
string(REPLACE "<iteration_steps>70</iteration_steps>"
               "<iteration_steps>0</iteration_steps>" NO_RESULT_PROBLEM
               "${QUICKSIM_PROBLEM}")
file(CONFIGURE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/quicksim_no_results.xml"
     CONTENT "${NO_RESULT_PROBLEM}" @ONLY)
string(REPLACE "l=\"0\"" "l=\"256\"" INVALID_BASIS_PROBLEM
               "${QUICKSIM_PROBLEM}")
file(CONFIGURE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/quicksim_invalid_basis.xml"
     CONTENT "${INVALID_BASIS_PROBLEM}" @ONLY)
add_test(
  NAME quicksim_no_results
  COMMAND
    ${CMAKE_COMMAND} "-DPLUGIN=$<TARGET_FILE:quicksim>"
    "-DINPUT=${CMAKE_CURRENT_BINARY_DIR}/quicksim_no_results.xml"
    "-DOUTPUT=${CMAKE_CURRENT_BINARY_DIR}/quicksim_no_results_out.xml"
    "-DEXPECTED_EXIT=1"
          "-DEXPECTED_MESSAGE=Simulation produced no charge distributions" -P
    "${CMAKE_CURRENT_SOURCE_DIR}/check_plugin_result.cmake")
add_test(
  NAME quicksim_invalid_basis
  COMMAND
    ${CMAKE_COMMAND} "-DPLUGIN=$<TARGET_FILE:quicksim>"
    "-DINPUT=${CMAKE_CURRENT_BINARY_DIR}/quicksim_invalid_basis.xml"
    "-DOUTPUT=${CMAKE_CURRENT_BINARY_DIR}/quicksim_invalid_basis_out.xml"
    "-DEXPECTED_EXIT=1"
          "-DEXPECTED_MESSAGE=Invalid SiQAD lattice basis index" -P
    "${CMAKE_CURRENT_SOURCE_DIR}/check_plugin_result.cmake")

foreach(plugin IN ITEMS quicksim quickexact clustercomplete)
  add_test(NAME ${plugin}_debug
           COMMAND ${CMAKE_COMMAND} "-DPLUGIN=$<TARGET_FILE:${plugin}>"
                   "-DINPUT=${CMAKE_CURRENT_SOURCE_DIR}/${plugin}_problem_0.xml"
                   "-DOUTPUT=${CMAKE_CURRENT_BINARY_DIR}/${plugin}_debug_out.xml"
                   "-DARGUMENTS=--debug" "-DEXPECTED_EXIT=0"
                   "-DEXPECTED_MESSAGE=--debug: Showing additional outputs"
                   -P "${CMAKE_CURRENT_SOURCE_DIR}/check_plugin_result.cmake")
endforeach()
