option(NOTARIZE "Run notarization scripts" OFF)

set(CPACK_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/_CPackConfig.cmake")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/deployment/PackageReadme.rtf" PackageReadme.rtf)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/deployment/CPackConfig.in.cmake" "${CPACK_CONFIG_FILE}" @ONLY)

get_filename_component(PRE_PACKAGE_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/PrePackage.cmake" ABSOLUTE)
get_filename_component(POST_PACKAGE_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/PostPackage.cmake" ABSOLUTE)

include("${CPACK_CONFIG_FILE}")

add_custom_target(package
        COMMAND ${CMAKE_COMMAND}
        -DNOTARIZE=${NOTARIZE}
        -P "${PRE_PACKAGE_SCRIPT}"

        COMMAND ${CMAKE_CPACK_COMMAND} --config "${CPACK_CONFIG_FILE}"

        COMMAND ${CMAKE_COMMAND}
        -DCPACK_PACKAGE_DIRECTORY=${CPACK_PACKAGE_DIRECTORY}
        -DNOTARIZE=${NOTARIZE}
        -P "${POST_PACKAGE_SCRIPT}"

        DEPENDS ${PACKAGING_DEPENDS_TARGETS})
