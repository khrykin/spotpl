//
// Created by Dmitry Khrykin on 03.08.2020.
//

#include <iomanip>
#include <iostream>

#include "errors.h"
#include "finder.h"
#include "playlist.h"
#include "utility.h"

namespace spotpl {

    inline constexpr auto *cant_write_message = "Can't write to specified output folder";

    using seconds = decltype(track::duration_seconds);

    inline auto get_artist(const nlohmann::json &track_json) -> std::string {
        const auto &album_artist = track_json["album"]["artists"][0]["name"];
        return album_artist == "Various Artists"
                   ? track_json["artists"][0]["name"]
                   : album_artist;
    }

    auto track::from_json(const nlohmann::json &item_json) -> track {
        auto &track_json = item_json["track"];
        auto duration_seconds = static_cast<seconds>(track_json["duration_ms"]) / 1000;

        return track{track_json["name"],
                     get_artist(track_json),
                     track_json["album"]["name"],
                     track_json["external_urls"]["spotify"],
                     duration_seconds};
    }

    auto playlist::from_json(const std::string &url,
                             const nlohmann::json &playlist_json,
                             const nlohmann::json &tracks_json) -> playlist {
        const auto &items_json = tracks_json["items"];

        std::vector<track> tracks(items_json.size());
        std::transform(items_json.begin(), items_json.end(),
                       tracks.begin(), track::from_json);

        return playlist{url, playlist_json["name"], tracks};
    }

    std::ostream &operator<<(std::ostream &os, const track &track) {
        auto pad = std::string(6, ' ');
        os << "track("
           << "name: \"" << track.name << "\"\n"
           << pad << "artist: \"" << track.artist << "\"\n"
           << pad << "album: \"" << track.album << "\"\n"
           << pad << "duration: " << track.duration_seconds << ")";
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const std::vector<track> &tracks) {
        for (auto &track : tracks) os << track << "\n";
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const playlist &playlist) {
        auto pad = std::string(9, ' ');
        os << "playlist("
           << "url: \"" << playlist.url << "\"\n"
           << pad << "name: \"" << playlist.name << "\"\n"
           << pad << "tracks count: " << playlist.tracks.size() << ")";
        return os;
    }

    void playlist::write_to_m3u(const std::string &library_path,
                                const std::string &output_path) {
        auto abs_output_path = filesystem::absolute(output_path);
        auto m3u_path = abs_output_path / (name + ".m3u");
        auto not_found_path = abs_output_path / (name + " - Not Found.txt");

        auto finder = spotpl::finder(*this, library_path);
        finder.find_all_tracks();

        if (!finder.found().empty()) {
            std::ofstream m3u;
            m3u.open(m3u_path.string());

            if (m3u.is_open()) {
                m3u << "#EXTM3U";

                for (auto &[track, path] : finder.found()) {
                    m3u << "\n#EXTINF:"
                        << track.duration_seconds << ","
                        << track.name << " - "
                        << track.artist << "\n"
                        << path.string();
                }

                m3u.close();

                std::cout << ".m3u playlist has been written to:\n\n\t"
                          << m3u_path << "\n\n";
            } else {
                throw error::generic(cant_write_message);
            }
        }

        if (!finder.not_found().empty()) {
            std::ofstream not_found_file;
            not_found_file.open(not_found_path.string());

            if (not_found_file.is_open()) {
                for (auto &track : finder.not_found()) {
                    not_found_file << track.url;

                    if (&track != &finder.not_found().back())
                        not_found_file << "\n";
                }

                not_found_file.close();

                std::cout << "List of not found tracks has been written to:\n\n\t"
                          << not_found_path << "\n\n";
            } else {
                throw error::generic(cant_write_message);
            }
        }
    }

}