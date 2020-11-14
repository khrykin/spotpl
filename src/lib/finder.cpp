//
// Created by Dmitry Khrykin on 03.08.2020.
//

#include <iostream>
#include <regex>
#include <utf8proc.h>

#include "finder.h"

namespace spotpl {

    namespace fs = filesystem;

    // TODO: Marked for deletion
    //    inline auto filtered_dirs(const fs::path &base_path,
    //                              const std::string &name) -> std::vector<fs::path> {
    //        std::vector<fs::path> result;
    //
    //        for (const auto &entry : fs::directory_iterator(base_path)) {
    //            if (!fs::is_directory(entry))
    //                continue;
    //
    //            auto normalized_filename = fs::normalize(entry.path().stem().string());
    //            auto normalized_name = fs::normalize(name);
    //
    //            if (normalized_filename.find(normalized_name) != std::string::npos) {
    //                result.push_back(fs::absolute(entry.path()));
    //            }
    //        }
    //
    //        return result;
    //    }

    inline auto grab_all_albums(const std::vector<fs::path> &artist_paths) -> std::vector<fs::path> {
        std::vector<fs::path> album_paths = {};

        for (const auto &artist_path : artist_paths) {
            for (const auto &entry : fs::directory_iterator(artist_path)) {
                if (!fs::is_directory(entry))
                    continue;

                album_paths.push_back(fs::absolute(entry.path()));
            }
        }

        return album_paths;
    }

#pragma mark - Constructions

    finder::finder(const spotpl::playlist &playlist, path library_path)
        : playlist(playlist),
          library_path(std::move(library_path)) {
    }

#pragma mark - Finding Tracks in a Local Library

    void finder::find_all_tracks() {
        for (const auto &track : playlist.tracks) {
            auto index = &track - &*playlist.tracks.begin() + 1;
            std::cout << "\rProcessing tracks: "
                      << index << " / " << playlist.tracks.size();

            auto track_path = find_track(track);

            if (track_path) {
                _found.push_back({track, *track_path});
            } else {
                _not_found.push_back(track);
            }
        }

        std::cout << "\r                                    "
                  << "\r";

        std::cout << "\nUsing local music library at:\n\n\t"
                  << "" << fs::absolute(library_path) << "\n";

        if (!_found.empty()) {
            std::cout << "\nFound " << _found.size() << "/" << playlist.tracks.size() << " "
                      << "tracks from Spotify playlist:\n\n\t"
                      << playlist.name << "\n\n\t"
                      << playlist.url << "\n\n";
        }

        if (!not_found().empty()) {
            std::cout << "The following tracks haven't been found:\n\n";

            for (auto &track : _not_found) {
                std::cout << "\t" << track.artist << " - " << track.name << " (album: " << track.album << ")";

                if (&track != &_not_found.back())
                    std::cout << "\n";
            }

            std::cout << "\n\n";
        }
    }

    auto finder::find_track(const track &track) -> std::unique_ptr<path> {
        namespace fs = filesystem;

        if (various_artists_cache.empty()) {
            populate_various_artists_cache();
        }

        if (artists_cache.empty()) {
            populate_artists_cache();
        }

        std::unique_ptr<fs::path> track_path;

        auto normalized_track_name = fs::normalize(track.name);

        auto artist_paths = finder::artist_paths(track.artist);
        auto album_paths = grab_all_albums(artist_paths);

        // TODO: Marked for deletion
        //    std::vector<fs::path> album_paths;
        //    for (const auto &artist_path: artist_paths) {
        //        auto albums = filtered_dirs(artist_path, track.album);
        //        album_paths.insert(album_paths.end(), albums.begin(), albums.end());
        //    }

        for (const auto &album_path : album_paths) {
            for (const auto &entry : fs::directory_iterator(album_path)) {
                auto normalized_filename = fs::normalize(entry.path().stem().string());

                if (normalized_filename.find(normalized_track_name) != std::string::npos) {
                    track_path = std::make_unique<fs::path>(fs::absolute(entry.path()));
                    break;
                }
            }
        }

        // Last resort - search in special locations
        if (!track_path) {
            if (various_artists_cache.find(fs::normalize(track.name)) != various_artists_cache.end()) {
                track_path = std::make_unique<path>(various_artists_cache[normalized_track_name]);
            }
        }

        return track_path;
    }

#pragma mark - Accessing Found and Unfound Tracks

    auto finder::found() const -> const std::vector<result> & {
        return _found;
    }

    auto finder::not_found() const -> const std::vector<track> & {
        return _not_found;
    }

#pragma mark - Populating Caches

    void finder::populate_artists_cache() {
        for (const auto &entry : fs::directory_iterator(library_path)) {
            if (!fs::is_directory(entry))
                continue;

            auto normalized_name = fs::normalize(entry.path().filename().string());

            if (artists_cache.find(normalized_name) == artists_cache.end())
                artists_cache[normalized_name] = {};

            artists_cache[normalized_name].push_back(fs::absolute(entry.path()));
        }
    }

    void finder::populate_various_artists_cache() {
        const std::vector<std::string> special_artists = {"Various Artists",
                                                          "Compilations"};

        for (auto &artist : special_artists) {
            auto path = library_path / artist;

            if (!fs::exists(path))
                continue;

            for (const auto &entry : fs::recursive_directory_iterator(path)) {
                if (!fs::is_directory(entry) || entry.path().stem().string()[0] == '.')
                    continue;

                auto normalized_name = fs::normalize(entry.path().stem().string());
                various_artists_cache[normalized_name] = fs::absolute(entry.path());
            }
        }
    }

#pragma mark - Getting Cached Artist Paths

    auto finder::artist_paths(const std::string &artist) -> std::vector<path> {
        auto normalized_artist = fs::normalize(artist);

        if (artists_cache.find(normalized_artist) != artists_cache.end())
            return artists_cache[normalized_artist];

        return {};
    }

#pragma mark - Stream Output

    auto operator<<(std::ostream &os, const finder::result &result) -> std::ostream & {
        os << "track: " << result.track << " path: " << result.path;
        return os;
    }

    auto operator<<(std::ostream &os, const std::vector<finder::result> &results) -> std::ostream & {
        for (auto &result : results) os << result << "\n";
        return os;
    }

}