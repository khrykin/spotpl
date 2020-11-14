//
// Created by Dmitry Khrykin on 08.10.2020.
//

#include <cxxopts.hpp>

#include "api.h"
#include "cli.h"
#include "errors.h"
#include "utility.h"

namespace spotpl::cli {

    struct parse_result {
        std::string library_path;
        std::string playlist_url;
        std::string output_path;

        friend std::ostream &operator<<(std::ostream &os, const parse_result &result) {
            os << "cli_parse_result("
               << "library_path: \"" << result.library_path << "\","
               << " output_path: \"" << result.output_path << "\","
               << " playlist_url: \"" << result.playlist_url << "\""
               << ")";

            return os;
        }
    };

    auto &options_setup() {
        static auto setup = ([] {
            using namespace cxxopts;

            auto options = Options("spotpl",
                                   "Creates .m3u playlist file from Spotify playlist url, "
                                   "using files from local music library.");

            auto &&add_option = options.add_options();

            add_option("l,library",
                       "Path to iTunes Music-like folder with Artist/Album/Tracks structure.",
                       value<std::string>());

            add_option("p,playlist",
                       "Spotify playlist URL",
                       value<std::string>());

            add_option("o,output_path",
                       "Path to the output folder",
                       value<std::string>()->default_value(""));

            add_option("v,version",
                       "Print current version");

            add_option("h,help",
                       "Print usage");

            return options;
        })();

        return setup;
    }

    inline auto parse_options(int argc, char **argv) -> optional<parse_result> {
        auto result = options_setup().parse(argc, argv);

        if (result.count("help")) {
            std::cout << options_setup().help() << std::endl;
            return nullopt;
        }

        if (result.count("version")) {
            std::cout << spotpl::version << std::endl;
            return nullopt;
        }

        if (!result.count("library")) {
            throw error::parse("You must provide library path");
        }

        if (!result.count("playlist")) {
            throw error::parse("You must provide Spotify playlist URL");
        }

        auto library_path = filesystem::expand_user(result["library"].as<std::string>());
        auto output_path = filesystem::expand_user(result["output_path"].as<std::string>());

        filesystem::validate_path(library_path);
        filesystem::validate_path(output_path);

        return parse_result{library_path,
                            result["playlist"].as<std::string>(),
                            output_path};
    }

    int exec(int argc, char **argv) {
        try {
            if (auto settings = parse_options(argc, argv)) {
                auto &[library_path,
                       playlist_url,
                       output_path] = *settings;

                auto pl = api::fetch_playlist(playlist_url);
                pl.write_to_m3u(library_path, output_path);
            }
        } catch (const std::exception &e) {
            error::print(e);
        }

        return 0;
    }
}