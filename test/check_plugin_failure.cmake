execute_process(COMMAND "${PLUGIN}" "${INPUT}" "${OUTPUT}"
                RESULT_VARIABLE status OUTPUT_VARIABLE output ERROR_VARIABLE error)
if(NOT status STREQUAL "1" OR NOT "${output}${error}" MATCHES "${EXPECTED_MESSAGE}")
  message(FATAL_ERROR "Expected exit 1 and '${EXPECTED_MESSAGE}', got ${status}:\n${output}${error}")
endif()
