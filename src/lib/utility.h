//
// Created by Dmitry Khrykin on 05.10.2020.
//

#ifndef SPOTPL_UTILITY_H
#define SPOTPL_UTILITY_H

#include <boost/filesystem.hpp>
#include <boost/optional.hpp>

namespace spotpl {

#pragma mark - Version

    extern const char *version;

#pragma mark - Strings

    auto utf8_fold_case(const std::string &str) -> std::string;
    auto wstring_from_utf8_string(const std::string &str) -> std::wstring;
    auto utf8_string_from_wstring(const std::wstring &wstr) -> std::string;

#pragma mark - Optional

    template<typename T>
    using optional = boost::optional<T>;

    inline const auto nullopt = boost::none;

#pragma mark - Filesystem

    namespace filesystem {
        using namespace boost::filesystem;

        void validate_path(const std::string &path) noexcept(false);
        auto normalize(std::string path_fragment) -> std::string;
        auto expand_user(std::string path) -> std::string;
    }

}

#endif//SPOTPL_UTILITY_H
