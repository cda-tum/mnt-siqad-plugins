file(REMOVE "${OUTPUT}")
execute_process(
  COMMAND "${PLUGIN}" "${INPUT}" "${OUTPUT}" ${ARGUMENTS}
  RESULT_VARIABLE status
  OUTPUT_VARIABLE output
  ERROR_VARIABLE error)
if(NOT status STREQUAL "${EXPECTED_EXIT}" OR NOT "${output}${error}" MATCHES
                                             "${EXPECTED_MESSAGE}")
  message(
    FATAL_ERROR
      "Expected exit ${EXPECTED_EXIT} and '${EXPECTED_MESSAGE}', got ${status}:\n${output}${error}"
  )
endif()
if(EXPECTED_EXIT EQUAL 0 AND NOT EXISTS "${OUTPUT}")
  message(FATAL_ERROR "Successful simulation did not write an output XML file")
elseif(NOT EXPECTED_EXIT EQUAL 0 AND EXISTS "${OUTPUT}")
  message(FATAL_ERROR "Failed simulation wrote an output XML file")
endif()
