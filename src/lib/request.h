//
// Created by Dmitry Khrykin on 03.08.2020.
//

#ifndef SPOTPL_REQUEST_H
#define SPOTPL_REQUEST_H

#include <map>
#include <memory>
#include <regex>
#include <string>

#include <boost/beast/core/detail/base64.hpp>
#include <boost/beast/http.hpp>

namespace web {
    namespace beast = boost::beast;// from <boost/beast.hpp>
    namespace http = beast::http;  // from <boost/beast/http.hpp>

    using method = http::verb;

#pragma mark - Base64

    inline auto base64encode(const std::string &string) -> std::string {
        using namespace boost::beast::detail;

        auto base64_max_size = base64::encoded_size(string.size());
        auto base64_str = std::unique_ptr<char[]>(new char[base64_max_size]);

        auto base64_size = base64::encode(base64_str.get(), string.c_str(), string.size());

        return std::string(base64_str.get(), base64_size);
    }

#pragma mark - Making Requests

    class request {
    public:
        explicit request(const std::string &url,
                         web::method method = http::verb::get);

        void make();

        template<typename... Params>
        void set(Params &&... params) {
            req.set(std::forward<Params>(params)...);
        }

        auto body() -> std::string &;
        auto response_text() -> std::string;

    private:
        std::string port;
        std::string host;
        std::string target;

        http::request<http::string_body> req;
        http::response<http::string_body> res;
    };
}


#endif//SPOTPL_REQUEST_H
