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
