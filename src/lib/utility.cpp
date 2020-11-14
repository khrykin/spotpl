//
// Created by Dmitry Khrykin on 08.10.2020.
//

#include <regex>

#include <utf8cpp/utf8.h>
#include <utf8proc.h>

#include "errors.h"
#include "utility.h"

namespace spotpl {

#pragma mark - Strings

    auto utf8_fold_case(const std::string &str) -> std::string {
        auto *c_str = reinterpret_cast<const utf8proc_uint8_t *>(str.c_str());
        auto *lowered = reinterpret_cast<char *>(utf8proc_NFKC_Casefold(c_str));

        return std::unique_ptr<char>(lowered).get();
    }

    auto wstring_from_utf8_string(const std::string &str) -> std::wstring {
        std::wstring res;

#ifdef _WIN32
        utf8::utf8to16(str.begin(), str.end(), std::back_inserter(res));
#else
        utf8::utf8to32(str.begin(), str.end(), std::back_inserter(res));
#endif

        return res;
    }

    auto utf8_string_from_wstring(const std::wstring &wstr) -> std::string {
        std::string res;

#ifdef _WIN32
        utf8::utf16to8(wstr.begin(), wstr.end(), std::back_inserter(res));
#else
        utf8::utf32to8(wstr.begin(), wstr.end(), std::back_inserter(res));
#endif

        return res;
    }

#pragma mark - Filesystem

    auto filesystem::normalize(std::string path_fragment) -> std::string {
        path_fragment = utf8_fold_case(path_fragment);

        auto wpath = wstring_from_utf8_string(path_fragment);

        // NB! Order of the following regex_replace's is important!

        // "&"s
        wpath = std::regex_replace(wpath, std::wregex(LR"(\&)"), L"and");
        //    wpath = std::regex_replace(wpath, std::wregex(LR"(\s*\([^\)]*\)$)"), L"");

        // Underscores
        wpath = std::regex_replace(wpath, std::wregex(LR"(_)"), L"'");

        // Remaster info, etc.
        wpath = std::regex_replace(wpath, std::wregex(LR"(\s*[\(\[\-].*(edit|mix|version|remaster|reissue).*$)"), L"");

        // Unwanted charaters
        wpath = std::regex_replace(wpath, std::wregex(LR"([^\d\w\s])", std::regex::collate), L"");

        // Suffixes (eg. - Remix)
        wpath = std::regex_replace(wpath, std::wregex(LR"(^\d+\s*-?\s*\d*\.?\s*)"), L"");

        // British/American English
        wpath = std::regex_replace(wpath, std::wregex(LR"(our(?!\w))"), L"or");

        // Extra whitespaces
        wpath = std::regex_replace(wpath, std::wregex(LR"(\s{2,})"), L" ");

        return utf8_string_from_wstring(wpath);
    }

    void filesystem::validate_path(const std::string &path) noexcept(false) {
        if (!filesystem::exists(path))
            throw error::generic("Path \"" + path + "\" doesn't exist.");

        if (!filesystem::is_directory(path))
            throw error::generic("Path \"" + path + "\" is not a directory.");
    }

    auto filesystem::expand_user(std::string path) -> std::string {
#ifdef _WIN32
        return path;
#else
        std::string home = std::getenv("HOME");
        auto it = path.find('~');
        if (it == std::string::npos)
            return path;

        return path.replace(0, it + 1, home);
#endif
    }
}