cmake_minimum_required(VERSION 3.16)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

include(GetVersionFromGit)

project(spotpl VERSION ${VERSION})

set(CMAKE_CXX_STANDARD 17)

# Dependencies

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.73.0 REQUIRED filesystem)
find_package(OpenSSL 1.1.1 REQUIRED)
find_package(Catch2 2.9 REQUIRED)
find_package(utf8proc 2.4 REQUIRED)
find_package(cxxopts 2.2.0 REQUIRED)
find_package(nlohmann_json 3.2 REQUIRED)
find_package(utf8cpp 3.1.2 REQUIRED)

# Compile options

if (MSVC)
    add_compile_options(/W4 /WX /wd4068)
else ()
    add_compile_options(-Wall -Wextra -pedantic -Werror -Wno-unknown-pragmas)
endif ()

# Reading credentials

configure_file(src/lib/credentials.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/credentials.cpp)

# Libraries

add_library(libspotpl STATIC
        src/lib/errors.cpp
        src/lib/errors.h
        src/lib/request.h
        src/lib/request.cpp
        src/lib/cli.h
        src/lib/cli.cpp
        src/lib/api.h
        src/lib/api.cpp
        src/lib/playlist.cpp
        src/lib/playlist.h
        src/lib/finder.cpp
        src/lib/finder.h
        src/lib/utility.h
        src/lib/utility.cpp)

target_include_directories(libspotpl PUBLIC src/lib)

target_link_libraries(libspotpl
        PRIVATE
        Boost::headers
        Boost::filesystem
        OpenSSL::SSL
        OpenSSL::Crypto
        utf8proc::utf8proc
        utf8::cpp
        cxxopts::cxxopts
        nlohmann_json::nlohmann_json)

if (WIN32)
    target_link_libraries(libspotpl
            PRIVATE
            crypt32)
endif ()

set_target_properties(libspotpl
        PROPERTIES
        OUTPUT_NAME spotpl
        MSVC_RUNTIME_LIBRARY MultiThreaded)

target_compile_definitions(libspotpl
        PRIVATE
        UTF8PROC_STATIC)


# Executables

add_executable(spotpl
        ${CMAKE_CURRENT_BINARY_DIR}/credentials.cpp
        src/cli/main.cpp)

set_target_properties(spotpl
        PROPERTIES
        MSVC_RUNTIME_LIBRARY MultiThreaded)

target_link_libraries(spotpl PRIVATE libspotpl)

add_executable(tests
        tests/tests_main.cpp
        tests/finder_test.cpp)

target_link_libraries(tests
        PRIVATE
        libspotpl
        Catch2::Catch2)

set_target_properties(tests
        PROPERTIES
        MSVC_RUNTIME_LIBRARY MultiThreaded)

# Coverage

if (CMAKE_BUILD_TYPE MATCHES Debug
        AND CMAKE_CXX_COMPILER_ID MATCHES Clang)
    option(COVERAGE "Compile with coverage" OFF)

    if (COVERAGE)
        target_compile_options(libspotpl PUBLIC -fprofile-instr-generate -fcoverage-mapping)
        target_link_options(libspotpl PUBLIC -fprofile-instr-generate -fcoverage-mapping)
    endif ()
endif ()

# Sanitizers

include(Sanitizers)

# Test suites

enable_testing()

include(Catch)

catch_discover_tests(tests)

# Installation

install(TARGETS spotpl
        CONFIGURATIONS Release
        COMPONENT Runtime
        RUNTIME DESTINATION bin)

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt"
        CONFIGURATIONS Release
        COMPONENT Runtime
        DESTINATION info/spotpl)

# Packaging

set(PACKAGING_DEPENDS_TARGETS spotpl)

include(Packaging)

