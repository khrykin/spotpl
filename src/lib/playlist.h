//
// Created by Dmitry Khrykin on 03.08.2020.
//

#ifndef SPOTPL_PLAYLIST_H
#define SPOTPL_PLAYLIST_H

#include <ostream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

namespace spotpl {

    struct track {
        static auto from_json(const nlohmann::json &item_json) -> track;

        std::string name;
        std::string artist;
        std::string album;
        std::string url;

        unsigned int duration_seconds;

        friend std::ostream &operator<<(std::ostream &os, const track &track);
        friend std::ostream &operator<<(std::ostream &os, const std::vector<track> &tracks);
    };

    struct playlist {
        static auto from_json(const std::string &url,
                              const nlohmann::json &playlist_json,
                              const nlohmann::json &tracks_json) -> playlist;

        std::string url;
        std::string name;
        std::vector<track> tracks;

        void write_to_m3u(const std::string &library_path,
                          const std::string &output_path);

        friend std::ostream &operator<<(std::ostream &os, const playlist &playlist);
    };

}

#endif//SPOTPL_PLAYLIST_H
