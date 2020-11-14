execute_process(COMMAND git describe --tags
        OUTPUT_VARIABLE FULL_VERSION
        WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
        OUTPUT_STRIP_TRAILING_WHITESPACE)

if (NOT FULL_VERSION)
    message(WARNING "No git tags was found. Setting version to 0.0.0")
    set(FULL_VERSION "0.0.0")
endif ()

string(REGEX REPLACE "^v" "" FULL_VERSION ${FULL_VERSION})
string(REGEX REPLACE "-([0-9]+)-.+" ".\\1" VERSION ${FULL_VERSION})


message(STATUS "Project version: ${VERSION}")
