if (APPLE AND NOTARIZE)
    set(CODESIGN_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/../../deployment/codesign.sh")

    execute_process(COMMAND "${CODESIGN_SCRIPT}" spotpl
            RESULT_VARIABLE CODESIGN_FAILED)

    if (CODESIGN_FAILED)
        message(FATAL_ERROR "Signing code failed")
    endif ()
endif ()