if (APPLE AND NOTARIZE)
    file(GLOB INSTALL_PACKAGE "${CPACK_PACKAGE_DIRECTORY}/*.pkg")

    if (NOT DEFINED ENV{CODE_SIGN_MAC_INSTALLER_IDENTITY})
        message(FATAL_ERROR "CODE_SIGN_MAC_INSTALLER_IDENTITY environment variable must be set to appropriate identitiy")
    endif ()

    get_filename_component(NOTARIZATION_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/../../deployment/notarize.sh" ABSOLUTE)
    get_filename_component(INSTALL_PACKAGE "${INSTALL_PACKAGE}" ABSOLUTE)

    execute_process(COMMAND "${NOTARIZATION_SCRIPT}" com.khrykin.spotpl ${INSTALL_PACKAGE}
            RESULT_VARIABLE CODESIGN_FAILED)

    if (CODESIGN_FAILED)
        message(FATAL_ERROR "Notarization failed")
    endif ()
endif ()