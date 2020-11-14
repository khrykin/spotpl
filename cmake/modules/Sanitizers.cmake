
if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
    option(ASAN "Enable Clang's AddressSanitizer" OFF)
    option(UBSAN "Enable Clang's UBSanitizer" OFF)
    option(TSAN "Enable Clang's ThreadSanitizer" OFF)

    if (NOT APPLE)
        option(LSAN "Enable Clang's LeakSanitizer" OFF)
    endif ()

    if (ASAN)
        message(STATUS "Using AddressSanitizer")

        target_compile_options(libspotpl PUBLIC -fsanitize=address)
        target_link_options(libspotpl PUBLIC -fsanitize=address)
    endif ()

    if (NOT APPLE AND LSAN)
        message(STATUS "Using LeakSanitizer")

        target_compile_options(libspotpl PUBLIC -fsanitize=leak)
        target_link_options(libspotpl PUBLIC -fsanitize=leak)
    endif ()

    if (UBSAN)
        message(STATUS "Using UndefinedBehaviorSanitizer")

        target_compile_options(libspotpl PUBLIC -fsanitize=undefined)
        target_link_options(libspotpl PUBLIC -fsanitize=undefined)
    endif ()

    if (TSAN)
        message(STATUS "Using ThreadSanitizer")

        target_compile_options(libspotpl PUBLIC -fsanitize=thread)
        target_link_options(libspotpl PUBLIC -fsanitize=thread)
    endif ()
endif ()