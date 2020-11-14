//
// Created by Dmitry Khrykin on 02.08.2020.
//

#ifndef SPOTPL_API_H
#define SPOTPL_API_H

#include "playlist.h"
#include "request.h"

namespace spotpl::api {

#pragma mark - Authorization

    extern const char *credentials;

    inline auto refresh_token() -> std::string {
        return "Basic " + web::base64encode(credentials);
    }

    inline auto auth_token(const std::string &base64_token) -> std::string {
        return "Bearer " + base64_token;
    }

    auto authorize() -> std::string;

#pragma mark - Fetching Playlist

    auto fetch_playlist(const std::string &item_json) -> playlist;
}

#endif//SPOTPL_API_H
