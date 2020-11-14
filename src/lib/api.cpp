//
// Created by Dmitry Khrykin on 03.08.2020.
//

#include <algorithm>
#include <iostream>
#include <nlohmann/json.hpp>

#include "api.h"
#include "errors.h"

namespace spotpl::api {
    constexpr auto invalid_playlist_error = "Invalid playlist URL";

#pragma mark - Authorization

    auto authorize() -> std::string {
        auto req = web::request("https://accounts.spotify.com/api/token", web::method::post);
        req.set("Authorization", refresh_token());

        req.body() = "grant_type=client_credentials";
        req.make();

        auto json = nlohmann::json::parse(req.response_text());

        if (json["access_token"].is_null()) {
            throw error::generic("Unable to authenticate with Spotify API");
        }

        return json["access_token"];
    }

#pragma mark - Fetching Playlist

    inline auto get_playlist_api_url(const std::string &playlist_url) {
        std::smatch playlist_id_match;
        if (!std::regex_search(playlist_url, playlist_id_match,
                               std::regex(R"(\/playlist\/([^\/]+))"))) {
            throw error::generic("Invalid playlist URL");
        }

        auto playlist_id = playlist_id_match[1].str();
        return "https://api.spotify.com/v1/playlists/" + playlist_id;
    }

    auto fetch_playlist(const std::string &url) -> playlist {
        auto access_token = api::auth_token(api::authorize());
        auto playlist_api_url = get_playlist_api_url(url);

        // Fetching playlist
        auto playlist_req = web::request(playlist_api_url);
        playlist_req.set("Authorization", access_token);
        playlist_req.make();

        auto playlist_json = nlohmann::json::parse(playlist_req.response_text());
        if (playlist_json["name"].is_null()) {
            throw error::generic(invalid_playlist_error);
        }

        // Fetching tracks
        auto tracks_req = web::request(playlist_api_url + "/tracks");
        tracks_req.set("Authorization", access_token);
        tracks_req.make();

        auto tracks_json = nlohmann::json::parse(tracks_req.response_text());
        if (tracks_json.is_null() || !tracks_json["items"].is_array()) {
            throw error::generic(invalid_playlist_error);
        }

        return playlist::from_json(url, playlist_json, tracks_json);
    }

}// namespace spotify