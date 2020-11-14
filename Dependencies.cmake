set(CMAKE_PROJECT_NAME spotpl)

set(PACKAGES
        boost-filesystem@0e12523
        boost-optional@0e12523
        boost-beast@0e12523
        openssl
        catch2
        utf8proc@726c111
        utfcpp
        cxxopts
        nlohmann-json)

if (WIN32)
    set(TRIPLETS
            x64-windows-static
            x86-windows-static)
endif ()