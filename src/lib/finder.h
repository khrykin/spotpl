//
// Created by Dmitry Khrykin on 03.08.2020.
//

#ifndef SPOTPL_FINDER_H
#define SPOTPL_FINDER_H

#include <ostream>
#include <string>
#include <unordered_map>

#include "playlist.h"
#include "utility.h"

namespace spotpl {

    class finder {
    public:
        using path = filesystem::path;

        struct result {
            spotpl::track track;
            filesystem::path path;
        };

#pragma mark - Construction

        finder(const playlist &playlist, path library_path);

#pragma mark - Finding Tracks in a Local Library

        void find_all_tracks();
        auto find_track(const track &track) -> std::unique_ptr<path>;

#pragma mark - Accessing Found and Unfound Tracks

        [[nodiscard]] auto found() const -> const std::vector<result> &;
        [[nodiscard]] auto not_found() const -> const std::vector<track> &;

    private:
        const spotpl::playlist &playlist;

        path library_path;

        std::vector<result> _found;
        std::vector<track> _not_found;

        std::unordered_map<std::string, std::vector<path>> artists_cache;
        std::unordered_map<std::string, path> various_artists_cache;

#pragma mark - Populating Caches

        void populate_artists_cache();
        void populate_various_artists_cache();

#pragma mark - Getting Cached Artist Paths

        auto artist_paths(const std::string &artist) -> std::vector<path>;
    };

#pragma mark - Stream Output

    auto operator<<(std::ostream &os, const finder::result &result) -> std::ostream &;
    auto operator<<(std::ostream &os, const std::vector<finder::result> &results) -> std::ostream &;
}

#endif//SPOTPL_FINDER_H
