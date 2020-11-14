# FindPackage file for static utf8roc

find_file(utf8cpp_HEADER NAMES utf8.h utf8cpp/utf8.h)

# Determine version:

if (NOT "${utf8cpp_HEADER}" MATCHES NOTFOUND)
    get_filename_component(utf8cpp_INCLUDE_DIR "${utf8cpp_HEADER}" DIRECTORY)

    if (NOT "${utf8cpp_HEADER}" MATCHES utf8cpp/utf8.h)
        set(utf8cpp_INCLUDE_DIR "${utf8cpp_INCLUDE_DIR}/utf8cpp")
        file(COPY "${utf8cpp_HEADER}" DESTINATION "${utf8cpp_INCLUDE_DIR}")
    endif ()
endif ()

# Handle find_package arguments:

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(utf8cpp
        REQUIRED_VARS
        utf8cpp_INCLUDE_DIR)

# Setup targets:

if (utf8cpp_FOUND AND NOT TARGET utf8cpp::utf8cpp)
    add_library(utf8::cpp INTERFACE IMPORTED)
    set_target_properties(utf8::cpp
            PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${utf8cpp_INCLUDE_DIR}")
endif ()